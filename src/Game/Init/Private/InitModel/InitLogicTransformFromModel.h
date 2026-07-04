// Initialize logical data from drawing data

#pragma once

// #include "Engine/Graphics/Private/Model/ModelData.h"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Engine/Public/EngineFwd.hpp"

#include "Types.hpp"

#include <string>


//namespace Game::Init::Logic2D
//{
//	// 3D -> 2D
//	Engine::ECS::Component::Logic2D::Logic2DTransformComponent InitLogic2DTransformFromModel(
//		const Engine::ECS::Component::Common::TransformComponent& transformComp
//		, const Engine::Graphics::Model::ModelData& modelData
//	);
//
//
//	// 3D -> 3D
//
//	// 2D -> 2D
//	//===============================
//	
//	// Get logical XZ size from model size, taking into account scale
//	// // スケールを考慮してモデルサイズから論理XZサイズを取得します
//	glm::vec2 GetModelXZSizeWithScale(const Engine::ECS::Component::Common::TransformComponent& transformComp, const Engine::Graphics::Model::ModelData& modelData);
//
//}
//
//// For Player's 2DCircle Collision form
namespace Game::Init::Logic2D
{
	// For initializing collision shape 
	// When the maximum axis of the model's size on the XZ plane is the radius
	// モデルのXZ平面における大きさの最大軸を半径にする場合

	float EstimateRadiusFromModelXZ(
		const Engine::Component::TransformComponent& transformComp,
		const Engine::Graphics::Model::ModelData& modelData,
		RadiusEstimateStrategy strategy = RadiusEstimateStrategy::MaxAxis
	);
}


//
//
