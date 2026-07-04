#include "TestBaseTerrainActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

// test color
#include "Engine/ECS/Component/Graphics/MaterialComponent.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Engine/ECS/Component/Tags/MapTags.h"

#include "Game/Init/Private/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/Private/InitTileMap/InitTileMap.h"

#include "Engine/Graphics/Private/Model/ModelData.h"

#include "Engine/Graphics/Private/Renderer/Shader.h"
#include "Engine/Graphics/Private/Model/AssimpImporter.h"
#include "Engine/Graphics/Private/Model/CgltfImporter.hpp"
#include "Engine/Graphics/Private/Renderer/GPUBufferUtils.h"

#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"



#include "Game/Actor/Map/TileMapActor.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <iostream>

Game::Actor::Map::TestBaseTerrainActor::TestBaseTerrainActor(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Shader* shader)
{
	//namespace Ops = Engine::ECS::Ops;
	//namespace Component = Engine::ECS::Component;
	//namespace Model = Engine::Graphics::Model;

	//Engine::ECS::Entity entity = ecs.createEntity();

	//// モデルデータインポート
	//// Engine::Graphics::Model::ModelData modelData = Engine::Graphics::Model::AssimpImporter::Import("Assets/Models/BaseMesh.fbx");
	//Engine::Graphics::Model::ModelData modelData = Model::CgltfImporter::Import("Assets/Models/test_terrain.glb");
	////ModelData modelData = AssimpImporter::Import("Assets/Models/HorizontallyTerrainMesh.fbx");
	//for (const auto& mesh : modelData.meshes)
	//{
	//	std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
	//		<< ", Indices: " << mesh.indices.size()
	//		<< ", hasIndices: " << mesh.hasIndices << std::endl;
	//}



	//// 初期描画座標を設定
	//Component::Common::TransformComponent transformComp;
	//transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	//transformComp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	//transformComp.scale = glm::vec3(1.0f);
	//Ops::Add<Component::Common::TransformComponent>(ecs, entity, transformComp);

	//// set ShaderComponent
	//Component::Graphics::ShaderComponent shaderComp;
	//shaderComp.shader = shader;
	//if (shaderComp.shader)
	//{
	//	shaderComp.shader->Use();
	//	shaderComp.shader->setMat4("model", transformComp.toMatrix());
	//	std::cout << "[TestBaseTerrainActor.cpp]: The model matrix was set successfully." << std::endl;
	//}
	//else
	//{
	//	std::cout << "[TestBaseTerrainActor.cpp]: Shader not found." << std::endl;
	//}
	//Ops::Add<Component::Graphics::ShaderComponent>(ecs, entity, shaderComp);

	//// 色情報を設定(デバッグ用)
	//Component::Graphics::MaterialComponent materialComp;
	//// materialComp.baseColor = glm::vec3(0.6f, 0.8f, 0.7f);
	//Engine::Debug::Logging::LogVector("TestBaseTerrainActor.cpp(Color)", materialComp.baseColor);
	//Ops::Add<Component::Graphics::MaterialComponent>(ecs, entity, materialComp);

	//// 描画情報から論理情報を初期化
	//Component::Logic2D::Logic2DTransformComponent logic2DComp;
	//logic2DComp = Game::Init::Logic2D::InitLogic2DTransformFromModel(transformComp, modelData);
	//Ops::Add<Component::Logic2D::Logic2DTransformComponent>(ecs, entity, logic2DComp);

	//std::cout << "TestBaseTerrainActor.cpp: Rotation " << logic2DComp.rotation << std::endl;




	//Component::Logic2D::CollisionComponent collisionComp;
	//// calc world size on the xz plane
	//glm::vec2 worldSize = Game::Init::Logic2D::GetModelXZSizeWithScale(transformComp, modelData);
	//// calc world center on the xz plane
	//glm::vec3 localCenter = modelData.GetCenter();
	//glm::vec3 worldCenter3D = transformComp.toMatrix() * glm::vec4(localCenter, 1.0f);
	//glm::vec2 worldCenterXZ = glm::vec2(worldCenter3D.x, worldCenter3D.z);
	//// calc local vector axisX and axisZ
	//float rotRad = logic2DComp.rotation;// 描画基準と論理基準の整合性を考える
	//// Front = Z axis basis
	//glm::vec2 axisZ = glm::normalize(Engine::Math::Logic2D::CalcForwardFromYaw((rotRad)));
	//glm::vec2 axisX = Engine::Math::Logic2D::CalcRightFromYaw(rotRad);

	//collisionComp.collider.shape = Component::Logic2D::Obb2D
	//{
	//	.center = glm::vec2(0.0f),// ローカルオフセット
	//	.halfExtents = worldSize * 0.5f,
	//	.axisX = axisX,
	//	.axisZ = axisZ
	//};
	//Ops::Add<Component::Logic2D::CollisionComponent>(ecs, entity, collisionComp);

	//Ops::Add<Component::Tags::TerrainMeshTag>(ecs, entity, Component::Tags::TerrainMeshTag{});

	//// TileMapActorの作成(Entityとして)
	//[[maybe_unused]] auto _ =  Game::Actor::Map::TileMapActor::Create(ecs, transformComp, modelData, 0.5f);

	//// GPUBufferをインポートデータから作成
	//Engine::Graphics::Model::ModelGPU modelGPU = Engine::Graphics::Render::GPUBufferUtils::createMeshGPUBuffers(modelData);
	//Ops::Add<Component::Graphics::MeshComponent>(ecs, entity,
	//	Component::Graphics::MeshComponent{ std::move(modelData), std::move(modelGPU) }
	//);

	//// 最終ログ
	//Engine::Debug::Logging::GeneralLog("TestBaseTerrainActor.cpp", "TestBaseTerrainActor creation completed successfully");
}

