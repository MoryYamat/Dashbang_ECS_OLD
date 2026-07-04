#include "InitLogicTransformFromModel.h"



#include "Game/Init/Public/InitApi.hpp"
#include "Game/Init/Private/InitModel/Types.hpp"
#include "Engine/Graphics/Private/Model/ModelData.h"
#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <iostream>

// Initialize 2D logical data from the position, rotation, and scale of a 3D model
// Engine::ECS::Component::Logic2D::Logic2DTransformComponent Game::Init::Logic2D::InitLogic2DTransformFromModel(
// 	const Engine::ECS::Component::Common::TransformComponent& transformComp
// 	, const Engine::Graphics::Model::ModelData& modelData
// )
// {
// 	Engine::ECS::Component::Logic2D::Logic2DTransformComponent logic;
// 
// 	// 位置データの初期化
// 	logic.positionXZ = glm::vec2(transformComp.position.x, transformComp.position.z);
// 
// 	// 回転データの初期化 (どうやってモデル正面を得るか)
// 	logic.rotation = glm::radians(transformComp.rotation.y);
// 	// logic.rotation = transformComp.rotation.y;// 
// 	logic.UpdateDirectionFromRotation();
// 
// 	// Get logical XZ size from model size and drawing scale
// 	logic.scale = GetModelXZSizeWithScale(transformComp, modelData);
// 
// 	//std::cout << "[InitLogicTransformSystem.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
// 	std::cout << "[InitLogicTransformSystem.cpp]: logical front vector x: " << logic.front.x << ", z: "<< logic.front.y << std::endl;
// 	std::cout << "[InitLogicTransformSystem.cpp]: logical rotationY: " << logic.rotation << std::endl;
// 	//std::cout << "[InitLogicTransformSystem.cpp]: logical right vector " << logic.right.x << std::endl;
// 	
// 	Engine::Debug::Logging::GeneralLog("InitLogicTransformFromModel.cpp",
// 		"Logical 2D coordinates were successfully initialized based on model drawing coordinates. ");
// 
// 	return logic;
// }
// 
// 
// 
// 
// glm::vec2 Game::Init::Logic2D::GetModelXZSizeWithScale(
// 	const Engine::ECS::Component::Common::TransformComponent& transformComp
// 	, const Engine::Graphics::Model::ModelData& modelData
// )
// {
// 	glm::vec2 logicalXZSize = glm::vec2(1.0f);
// 
// 	// モデルデータからサイズを取得
// 	glm::vec3 rowModelSize = modelData.GetSize();
// 
// 	// XZサイズをスケール付きで計算
// 	if (rowModelSize.x > 0.0001f && rowModelSize.z > 0.0001f)
// 	{
// 		logicalXZSize.x = transformComp.scale.x * rowModelSize.x;
// 		logicalXZSize.y = transformComp.scale.z * rowModelSize.z;
// 	}
// 	else
// 	{
// 		logicalXZSize = glm::vec2(1.0f);
// 		Engine::Debug::Logging::GeneralLog("InitLogicTransformFromModel(scale)", "Probably an error occurred while getting/setting the scale.");
// 	}
// 
// 	return logicalXZSize;
// }
// 
// 
// 
// // 削除予定
// float  Game::Init::Logic2D::EstimateRadiusFromModelXZ(
// 	const Engine::ECS::Component::Common::TransformComponent& transformComp
// 	, const Engine::Graphics::Model::ModelData& modelData
// 	, RadiusEstimateStrategy strategy
// )
// {
// 	if (strategy == RadiusEstimateStrategy::MaxAxis)
// 	{
// 		glm::vec2 worldSize = GetModelXZSizeWithScale(transformComp, modelData);
// 		return std::max(worldSize.x, worldSize.y) * 0.5f;
// 	}
// 	else if (strategy == RadiusEstimateStrategy::MinAxis)
// 	{
// 		glm::vec2 worldSize = GetModelXZSizeWithScale(transformComp, modelData);
// 		return std::min(worldSize.x, worldSize.y) * 0.5f;
// 	}
// 	else if (strategy == RadiusEstimateStrategy::Diagonal)
// 	{
// 		glm::vec2 worldSize = GetModelXZSizeWithScale(transformComp, modelData);
// 		return glm::length(worldSize) * 0.5f;
// 	}
// 	else
// 	{
// 		Engine::Debug::Logging::GeneralLog("InitLogicTransformFromModel.cpp(Radius)", "Maybe radius initialization failed");
// 		return 0.5f;
// 
// 	}
// }

namespace Game::Init::Logic2D
{

	Engine::Component::Logic2DTransformComponent InitLogic2DTransformFromModel(
		const Engine::Component::TransformComponent& transformComp
		, const Engine::Graphics::Model::ModelData& modelData
	)
	{
		Engine::Component::Logic2DTransformComponent logic;

		// 位置データの初期化
		logic.positionXZ = glm::vec2(transformComp.position.x, transformComp.position.z);

		// 回転データの初期化 (どうやってモデル正面を得るか)
		logic.rotation = glm::radians(transformComp.rotation.y);
		// logic.rotation = transformComp.rotation.y;// 
		logic.UpdateDirectionFromRotation();

		// Get logical XZ size from model size and drawing scale
		logic.scale = GetModelXZSizeWithScale(transformComp, modelData);

		std::cout << "[InitLogicTransformSystem.cpp]: logical front vector x: " << logic.front.x << ", z: " << logic.front.y << std::endl;
		std::cout << "[InitLogicTransformSystem.cpp]: logical rotationY: " << logic.rotation << std::endl;

		return logic;
	}

	glm::vec2 GetModelXZSizeWithScale(
		const Engine::Component::TransformComponent& transformComp,
		const Engine::Graphics::Model::ModelData& modelData
	)
	{
		glm::vec2 logicalXZSize = glm::vec2(1.0f);

		// モデルデータからサイズを取得
		glm::vec3 rowModelSize = modelData.GetSize();

		// XZサイズをスケール付きで計算
		if (rowModelSize.x > 0.0001f && rowModelSize.z > 0.0001f)
		{
			logicalXZSize.x = transformComp.scale.x * rowModelSize.x;
			logicalXZSize.y = transformComp.scale.z * rowModelSize.z;
		}
		else
		{
			logicalXZSize = glm::vec2(1.0f);
		}

		return logicalXZSize;
	}


	float EstimateRadiusFromModelXZ(
		const Engine::Component::TransformComponent& transformComp,
		const Engine::Graphics::Model::ModelData& modelData,
		RadiusEstimateStrategy strategy
	)
	{
		if (strategy == RadiusEstimateStrategy::MaxAxis)
		{
			glm::vec2 worldSize = GetModelXZSizeWithScale(transformComp, modelData);
			return std::max(worldSize.x, worldSize.y) * 0.5f;
		}
		else if (strategy == RadiusEstimateStrategy::MinAxis)
		{
			glm::vec2 worldSize = GetModelXZSizeWithScale(transformComp, modelData);
			return std::min(worldSize.x, worldSize.y) * 0.5f;
		}
		else if (strategy == RadiusEstimateStrategy::Diagonal)
		{
			glm::vec2 worldSize = GetModelXZSizeWithScale(transformComp, modelData);
			return glm::length(worldSize) * 0.5f;
		}
		else
		{
			return 0.5f;

		}
	}
}