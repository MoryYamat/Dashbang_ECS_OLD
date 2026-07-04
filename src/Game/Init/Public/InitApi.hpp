#pragma once

#include "Game/Init/Private/InitModel/Types.hpp"
#include "Engine/Public/EngineFwd.hpp"

namespace Game::Init::Logic2D
{
	// 3D -> 2D
	Engine::Component::Logic2DTransformComponent InitLogic2DTransformFromModel(
		const Engine::Component::TransformComponent& transformComp
		, const Engine::Graphics::Model::ModelData& modelData
	);

	// 3D -> 3D

	// 2D -> 2D

	// Get logical XZ size from model size, taking into account scale
	// // スケールを考慮してモデルサイズから論理XZサイズを取得します
	glm::vec2 GetModelXZSizeWithScale(const Engine::Component::TransformComponent& transformComp
		, const Engine::Graphics::Model::ModelData& modelData);

}