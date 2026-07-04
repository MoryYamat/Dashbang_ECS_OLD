#include "Engine/Graphics/Public/GraphicsApi.hpp"

#include "Engine/Graphics/Public/Types.hpp"

#include "Engine/Graphics/Internal/GraphicsLog.hpp"
#include "Engine/Graphics/Internal/GraphicsInternalTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/IO/Public/FileSystemAPI.hpp"

#include <algorithm>
#include <string>
#include <iostream>

namespace Engine::Graphics
{
	void RendererResources::Init(const std::string& vert, const std::string& frag)
	{
		if (initialized) return;
		spriteRenderer_.Init();		// VAO / VBO / EBO
		spriteShader_ = ::Engine::Graphics::Shader(vert, frag);
		initialized = true;
	}

	RendererResources::~RendererResources()
	{
		Shutdown();
	}

	void RendererResources::Shutdown()
	{
		if (!initialized) return;

		// GLコンテキストが生きている間に呼ぶ
		texMgr_.shutdown();
		spriteRenderer_.Shutdown();

		spriteShader_.reset();

		initialized = false;
	}

	void RendererResources::RenderFrame(
		::Engine::WorldSystem::Core::WorldCtx& ctx,
		const RenderContext& renderCTX,
		::Engine::IO::IPathResolver& resolver
	)
	{
		if (!initialized)
		{
			Log::warn(Log::kRenderFrame, "RenderResources is not initialized");
			return;
		}

		auto* vfxActive = ctx.ww.TryGetResource<::Engine::VFX::VFXActiveInstanceResource>();
		auto* vfxCatRes = ctx.ww.TryGetResource<::Engine::VFX::VFXCatalogResource>();
		if (!vfxActive || !vfxCatRes)
		{
			// Log::warn(Log::kRenderSystem, "VFXActiveInstanceResource or VFXCatalogResource cannot be found (may not be initialized)");
			return;
		}

		const auto& cat = vfxCatRes->catalog;

		for (const auto& inst : vfxActive->instances)
		{
			if (!inst.id.is_valid())
				continue;

			// vpath -> abs ( catalog で cache)
			auto absOpt = cat.get_or_resolve_abs_path(inst.id, resolver);
			if (!absOpt) continue;

			// absPath -> TextureHandle
			const TextureHandle th = texMgr_.get_or_load_texture(*absOpt);
			if (!th.is_valid())
				continue;

			const GLuint glTex = texMgr_.gl_id(th);
			if (glTex == 0)
				continue;

			// model matrix
			glm::mat4 model = ToGlmMat4(inst.trs);

			// 4) XY板を XZ 平面に寝かせる（スプライトが地面向きになる）
			// xy quad -> XZ Plane
			// glm::mat4 plane = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

			// model = model * plane;


			// fade (仮)
			float alpha = 1.0f;
			if (inst.ttl > 1e-6f)
				alpha = 1.0f - (inst.t / inst.ttl);

			alpha = std::clamp(alpha, 0.0f, 1.0f);

			// render
			spriteRenderer_.Draw(
				spriteShader_,
				renderCTX.viewMatrix,
				renderCTX.projectionMatrix,
				model,
				glTex,
				Engine::VFX::VFXBlend::Add, // 仮。後で runtime から取る
				glm::vec4(1, 1, 1, alpha)
			);
		}
	}
}