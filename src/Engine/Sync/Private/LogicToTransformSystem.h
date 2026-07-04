// Linking logic coordinates with drawing coordinates

#pragma once

#include "Engine/ECS/EntityManager.h"


// Logic -> Drawing
// ロジック　-> 描画
namespace Engine::Sync::LogicToTransformSystem
{
	// 2D -> 3D
	void Apply2DToTransform(Engine::ECS::EntityMgr& ecs, float deltaTime);

	// 3D -> 3D
	void Apply3DToTransform(Engine::ECS::EntityMgr& ecs, float deltaTime);


	// 2D -> 2D
	//void Apply2D
};