#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Combat
{
	
	class HitEventFeature
	{
	public:
		static void InitializeHitEventDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateHitEventManager(Engine::ECS::EntityMgr& ecs);

		//
		static void InitializeHitEventDatabase(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateHitEventManager(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}