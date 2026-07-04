#include "RenderSystem.h"

// 
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Graphics/Public/GraphicsApi.hpp"
#include "Engine/Graphics/Public/Types.hpp"

#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Graphics/MeshComponent.hpp"
#include "Engine/Component/Private/Camera/CameraComponent.hpp"
//

// graphics
#include "Engine/Graphics/Private/Renderer/OpenGLTextureManager.hpp"

// vfx
#include "Engine/VFX/Public/VFXAPI.hpp"

// IO
#include "Engine/IO/Public/FileSystemAPI.hpp"

// log
#include "Engine/Graphics/Internal/GraphicsLog.hpp"

// math
#include "Engine/Graphics/Internal/GraphicsInternalTypes.hpp"

#include "Engine/ECS/Entity.h"

#include "Engine/Component/Private/Graphics/AnimatorComponent.hpp"

// time
#include "Engine/Time/Private/WorldClock.hpp"

#include <glm/gtc/type_ptr.hpp>

#include<optional>
#include <iostream>

#define MAX_BONES 128

// 現在未使用
//void Engine::Graphics::Render::UpdateRenderContext(Engine::ECS::EntityMgr& ecs, Engine::Graphics::RenderContext& context)
//{
//	for (Engine::ECS::Entity e : ecs.view<
//		Engine::ECS::Component::Common::TransformComponent,
//		eNsCamComp::CameraComponent>())
//	{
//		const auto& transform = ecs.get<Engine::ECS::Component::Common::TransformComponent>(e);
//		const auto& cam = ecs.get<eNsCamComp::CameraComponent>(e);
//
//		// view / projection matrixの更新
//		glm::mat4 view = glm::lookAt(transform.position, transform.position + cam.front, cam.up);
//		glm::mat4 projection = glm::perspective(glm::radians(cam.fov), cam.aspect, cam.nearClip, cam.farClip);
//
//		// キャッシュ
//		context.cameraPosition = transform.position;
//		context.cameraFront = cam.front;
//		context.cameraRight = cam.right;
//		context.cameraUp = cam.up;
//
//		context.viewMatrix = view;
//		context.projectionMatrix = projection;
//
//		return;// 最初のカメラのみ使用
//	}
//
//	std::cerr << "[CameraSystem::UpdateRenderContext] : No camera found in ECS." << std::endl;
//}

// 一時ヘルパ
namespace
{

	// 最新版
	inline void drawOneSubmesh
	(const Engine::Component::MeshComponent& mc, size_t i)
	{
		const auto& mg = mc.modelGPU.meshesGPU[i];
		const auto& md = mc.modelData.meshes[i];
		glBindVertexArray(mg.vao);
		if (md.hasIndices) glDrawElements(GL_TRIANGLES, mg.indexCount, GL_UNSIGNED_INT, 0);
		else               glDrawArrays(GL_TRIANGLES, 0, mg.indexCount);
		glBindVertexArray(0);
	}
}

namespace Engine::Graphics
{
	static glm::mat4 computeViewMatrix(const Engine::Component::TransformComponent& transformComp, const Engine::Component::CameraComponent& cameraComp)
	{
		glm::vec3 position = transformComp.position;
		return glm::lookAt(position, position + cameraComp.front, cameraComp.up);
	}

	static glm::mat4 computeProjectionMatrix(float fov, float aspect, float nearClip, float farClip)
	{
		return glm::perspective(fov, aspect, nearClip, farClip);
	}

	static bool getCameraMatrices(Engine::WorldSystem::Core::WorldCtx& ctx, glm::mat4& view, glm::mat4& projection, RenderContext& context)
	{
		bool ok = false;

		auto ents = Engine::WorldSystem::Query::ViewWhere(ctx.rw,
			Engine::WorldSystem::Query::All<Engine::Component::TransformComponent, Engine::Component::CameraComponent>{});

		for (const auto& e : ents)
		{
			auto& ts = ctx.ww.Get<Engine::Component::TransformComponent>(e);
			auto& cam = ctx.ww.Get<Engine::Component::CameraComponent>(e);
			
			view = computeViewMatrix(ts, cam);
			projection = computeProjectionMatrix(cam.fov, cam.aspect, cam.nearClip, cam.farClip);

			context.cameraPosition = ts.position;
			context.cameraFront = cam.front;
			context.cameraRight = cam.right;
			context.cameraUp = cam.up;
			ok = true;
		}

		if (ok) return true;

		// カメラが見つからなかった
		std::cerr << "[RenderSystem.cpp(getCameraMatrices)]: No Camera found!" << std::endl;
		return false;
	}

	// 最新版
	void RenderSystem(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader& shader, std::span<RendererPipelineEntry> ents
		, float aspect, RenderContext& context)
	{
		// View行列、Projection行列
		glm::mat4 view, projection;
		// カメラ行列計算必要
		if (!getCameraMatrices(ctx, view, projection, context))
		{
			std::cerr << "[RenderSystem.cpp]: No valid camera found in ECS." << std::endl;
			return;
		}

		const auto& dt = ctx.ww.GetResource<::Engine::Time::WorldClockData>().frameDt;


		for (auto& e : ents)
		{
			auto* ms = ctx.ww.TryGet<Engine::Component::MeshComponent>(e.e);
			if (!ms) continue;

			//static bool onceGpu = false;
			//if (!onceGpu)
			//{
			//	std::cout << "[dbg] CPU meshes=" << ms->modelData.meshes.size()
			//		<< " GPU meshes=" << ms->modelGPU.meshesGPU.size() << "\n";

			//	for (size_t i = 0; i < ms->modelGPU.meshesGPU.size(); ++i)
			//	{
			//		const auto& g = ms->modelGPU.meshesGPU[i];
			//		std::cout << "[dbg] i=" << i
			//			<< " vao=" << g.vao
			//			<< " vbo=" << g.vbo
			//			<< " ebo=" << g.ebo
			//			<< " indexCount=" << g.indexCount
			//			<< " name='" << ms->modelData.meshes[i].source_name << "'\n";
			//	}
			//	onceGpu = true;
			//}

			auto& ts = ctx.ww.Get<Engine::Component::TransformComponent>(e.e);
			// auto& ms = ctx.ww.Get<Engine::Component::MeshComponent>(e.e);

			// state machine (シェーダーを切り替えると、viewもprojectionもセットする必要あり。)
			shader.Use();
			shader.setMat4("uModel", ts.toMatrix());
			// shader.setMat4("uModel", glm::mat4(1.0f));
			shader.setMat4("uView", view);
			shader.setMat4("uProjection", projection);

			// ボーンパレットを送る
			int boneCount = 0;
			if (ctx.rw.Has<Engine::Component::AnimatorComponent>(e.e))
			{
				const auto& anim = ctx.ww.Get<Engine::Component::AnimatorComponent>(e.e);
				boneCount = (int)anim.palette.size();
				if (boneCount > 0)
				{
					boneCount = std::min(boneCount, MAX_BONES); // vs -> MAX_BONES
					shader.setInt("uBoneCount", boneCount);
					// shader.setInt("uBoneCount", 0);// -> スキニング側の問題っぽい

					GLint loc = glGetUniformLocation(shader.GetProgram(), "uBones[0]");
					if (loc == -1) {
						std::cerr << "[RenderSystem] Warning: uBones[0] uniform not found (maybe optimized out)" << std::endl;
					}
					else {
						glUniformMatrix4fv(loc, boneCount, GL_FALSE, glm::value_ptr(anim.palette[0]));
						// std::cout << "[RenderSystem] Uploaded bone palette: " << boneCount << " matrices" << std::endl;
					}
				}
				else
				{
					shader.setInt("uBoneCount", 0);
				}
			}
			else
			{
				// アニメータなし -> スキニングなし
				shader.setInt("uBoneCount", 0);
			}
			// meshごとに素材を反映
			for (size_t i = 0; i < ms->modelData.meshes.size(); ++i)
			{
				const auto& meshData = ms->modelData.meshes[i];

				bool hasTex = false;
				GLuint texId = 0;

				// const bool isSword = (meshData.source_name.find("Sword") != std::string::npos);
				// if (!isSword) continue; // 
				// textures[0] を "diffuse" として扱う
				if (!meshData.materialData.textures.empty())
				{
					texId = meshData.materialData.textures[0].id;
					hasTex = (texId != 0);
				}

				shader.setBool("uHasBaseColorTex", hasTex);
				if (hasTex)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texId);
					shader.setInt("uBaseColorTex", 0);// sampler2D に ユニット0 を関連付け
				}

				// static float time = 0.f;
				// time += dt;

				// shader.setFloat("uWeaponVfxStrength", 1.0f);
				// shader.setFloat("uTime", time);

				// static bool once = false;
				// if (!once)
				// {
				// 	std::cout << "[dbg] i=" << i
				// 		<< " name='" << meshData.source_name << "'"
				// 		<< " isSword=" << (meshData.source_name.find("Sword") != std::string::npos)
				// 		<< "\n";
				// 	if (i + 1 == ms->modelData.meshes.size()) once = true;
				// }

				//if (i == 0 || i == 3)
				//{
				//	GLint curProg = 0;
				//	glGetIntegerv(GL_CURRENT_PROGRAM, &curProg);

				//	GLint loc = glGetUniformLocation(shader.GetProgram(), "uWeaponVfxStrength");
				//	float v = -999.0f;
				//	if (loc != -1)
				//		glGetUniformfv(shader.GetProgram(), loc, &v);

				//	std::cout << "[dbg] i=" << i
				//		<< " isSword=" << isSword
				//		<< " curProg=" << curProg
				//		<< " shaderProg=" << shader.GetProgram()
				//		<< " loc=" << loc
				//		<< " val=" << v
				//		<< "\n";
				//}


				// テクスチャが無いとき用の係数
				shader.setVec3("uBaseColor", meshData.materialData.baseColor);

				// 1サブメッシュだけ描画するヘルパ
				drawOneSubmesh(*ms, i);

				if (hasTex)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			//shader.setVec3("uBaseColor", materialComp.baseColor);
			// drawMesh(ms);
		}

		context.viewMatrix = view;
		context.projectionMatrix = projection;
	}
}
