// Special template for TileMapComponent initialization
// TileMapComponent初期化用の特殊テンプレート

#pragma once

#include "Engine/ECS/Meta/InitComponent/InitSystem.h"

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Game/Init/Private/InitTileMap/InitTileMap.h"


namespace Engine::ECS::Meta::Init
{
	template<>
	struct InitSystem<Engine::ECS::Component::Logic2D::TileMapComponent>
	{
		static void Init(Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp, Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity tileMapEntity, const Engine::Window::Window& window)
		{
			Game::Init::Logic2D::ApplyObstacleCollidersToTileMap(ecs, tileMapComp);
			Game::Init::Logic2D::MaskUncoveredTilesByTerrainOBB(ecs, tileMapComp);
		}

	};
}
