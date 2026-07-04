#include "TestRockActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"
#include "Engine/ECS/Component/Graphics/MaterialComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Engine/ECS/Component/Tags/ObstacleTagComponent.h"

#include "Engine/Graphics/Private/Model/ModelData.h"

#include "Game/Init/Private/InitModel/InitLogicTransformFromModel.h"

#include "Engine/Graphics/Private/Model/AssimpImporter.h"
#include "Engine/Graphics/Private/Model/CgltfImporter.hpp"
#include "Engine/Graphics/Private/Renderer/GPUBufferUtils.h"

#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"

#include "Game/Collision/Public/CollisionMaskComponent.hpp"



#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Engine/Graphics/Private/Renderer/Shader.h"

#include <iostream>

Game::Actor::Map::TestRockActor::TestRockActor(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Shader* shader)
{
	//namespace Ops = Engine::ECS::Ops;
	//namespace Component = Engine::ECS::Component;
	//namespace Model = Engine::Graphics::Model;

	//Engine::ECS::Entity entity = ecs.createEntity();

	//Engine::Graphics::Model::ModelData modelData = Engine::Graphics::Model::AssimpImporter::Import("Assets/Models/TestRock.fbx");
	//// Engine::Graphics::Model::ModelData modelData = Model::CgltfImporter::Import("Assets/Models/TestRock.fbx");
	//for (const auto& mesh : modelData.meshes)
	//{
	//	std::cout << "[TestRockActor.cpp]: Vertices: " << mesh.vertices.size()
	//		<< ", Indices: " << mesh.indices.size()
	//		<< ", hasIndices: " << mesh.hasIndices << std::endl;
	//}

	//// set Mesh data to GPUBuffers
	//Engine::Graphics::Model::ModelGPU modelGPU = Engine::Graphics::Render::GPUBufferUtils::createMeshGPUBuffers(modelData);


	//// set TransformComponent
	//Component::Common::TransformComponent transformComp;
	//transformComp.position = glm::vec3(10.0f, 0.0f, -2.0f);
	//transformComp.rotation = glm::vec3(0.0f, -60.0f, 0.0f);
	//transformComp.scale = glm::vec3(0.01f);
	//Ops::Add<Engine::ECS::Component::Common::TransformComponent>(ecs, entity, transformComp);


	//// set ShaderComponent
	//Component::Graphics::ShaderComponent shaderComp;
	//shaderComp.shader = shader;
	//if (shaderComp.shader)
	//{
	//	shaderComp.shader->Use();
	//	shaderComp.shader->setMat4("model", transformComp.toMatrix());
	//	std::cout << "[TestRockActor.cpp]: The model matrix was set successfully." << std::endl;
	//}
	//else
	//{
	//	std::cout << "[TestRockActor.cpp]: Shader not found." << std::endl;
	//}
	//Ops::Add<Engine::ECS::Component::Graphics::ShaderComponent>(ecs, entity, shaderComp);



	//// Logic2D
	//Component::Logic2D::Logic2DTransformComponent logic;
	//logic = Game::Init::Logic2D::InitLogic2DTransformFromModel(transformComp, modelData);
	//Ops::Add<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(ecs, entity, logic);


	//// set Test Corlor
	//Component::Graphics::MaterialComponent materialComp;
	//materialComp.baseColor = glm::vec3(1.0f, 0.0f, 1.0f);
	//Ops::Add<Engine::ECS::Component::Graphics::MaterialComponent>(ecs, entity, materialComp);

	//// for collision setting
	//Component::Logic2D::CollisionComponent testRockCollisionComp;
	//// calc world size on the xz plane
	//glm::vec2 obbSize = Game::Init::Logic2D::GetModelXZSizeWithScale(transformComp, modelData);
	//// calc world center on the xz plane
	//glm::vec3 localCenter = modelData.GetCenter();
	//glm::vec3 worldCenter3D = transformComp.toMatrix() * glm::vec4(localCenter, 1.0f);
	//glm::vec2 worldCenterXZ = glm::vec2(worldCenter3D.x, worldCenter3D.z);
	//// calc local vector axisX and axisZ
	//float rotRad = logic.rotation;// 描画基準(+Z)と論理基準(-Z)の整合性を考える
	//glm::vec2 axisZ = glm::normalize(Engine::Math::Logic2D::CalcForwardFromYaw(rotRad));
	//glm::vec2 axisX = Engine::Math::Logic2D::CalcRightFromYaw(rotRad);

	//testRockCollisionComp.collider.shape = Component::Logic2D::Obb2D{
	//	.center = glm::vec2(0.0f),// ローカルオフセット
	//	.halfExtents =obbSize / 2.0f,
	//	.axisX = axisX,
	//	.axisZ = axisZ
	//};

	//
	//Ops::Add<Component::Logic2D::CollisionComponent>(ecs, entity, testRockCollisionComp);
	//
	//// obstacle object's tag
	//Component::Tags::ObstacleTagComponent obstacleTagComp;
	//Ops::Add<Component::Tags::ObstacleTagComponent>(ecs, entity, obstacleTagComp);
 //
	//// set MeshComponent
	//Ops::Add<Engine::ECS::Component::Graphics::MeshComponent>(ecs, entity,
	//	Engine::ECS::Component::Graphics::MeshComponent{ std::move(modelData),std::move(modelGPU) });

	//std::cout << "[TestRockActor.cpp]: Test3Dmodel Settings Completed" << std::endl;
}