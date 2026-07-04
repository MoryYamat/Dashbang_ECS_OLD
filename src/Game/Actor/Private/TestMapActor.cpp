#include "Game/Actor/Public/TestMapActor.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Graphics/Public/Types.hpp"
#include "Engine/Graphics/Private/Model/CgltfImporter.hpp"
#include "Engine/Graphics/Private/Model/ModelData.h"
#include "Engine/Graphics/Private/Renderer/GPUBufferUtils.h"

#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Component/Private/Graphics/ShaderComponent.hpp"

#include "Game/Character/Animation/Public/LocomAnimComponent.hpp"

#include "Game/Init/Public/InitApi.hpp"

namespace Game::Actor
{
	using namespace Engine::WorldSystem;
	using namespace Engine::Graphics;
	using namespace Engine::Component;

	TestTerrainMesh::TestTerrainMesh(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader* shader)
	{
		Engine::ECS::Core::Entity e = ctx.ww.Create();
		
		Model::ModelData model = Model::CgltfImporter::Import("Assets/Models/test_terrain.glb");

		for (const auto& mesh : model.meshes)
		{
			std::cout << "[TestBaseTerrainActor.cpp]: Vertices: " << mesh.vertices.size()
				<< ", Indices: " << mesh.indices.size()
				<< ", hasIndices: " << mesh.hasIndices << std::endl;
		}

		TransformComponent& trans = ctx.ww.Add<TransformComponent>(e);

		ShaderComponent shaderComp;
		shaderComp.shader = shader;
		if (shaderComp.shader)
		{
			shaderComp.shader->Use();
			shaderComp.shader->setMat4("model", trans.toMatrix());
			std::cout << "[TestBaseTerrainActor.cpp]: The model matrix was set successfully." << std::endl;
		}
		else
		{
			std::cout << "[TestBaseTerrainActor.cpp]: Shader not found." << std::endl;
		}
		ctx.ww.Add<ShaderComponent>(e, shaderComp);

		Logic2DTransformComponent& logic = ctx.ww.Add<Logic2DTransformComponent>(e);
		logic = Game::Init::Logic2D::InitLogic2DTransformFromModel(trans, model);




		Model::ModelGPU modelGPU = Render::GPUBufferUtils::createMeshGPUBuffers(model);
		ctx.ww.Add<MeshComponent>(e, MeshComponent{ std::move(model), std::move(modelGPU) });


	}
}