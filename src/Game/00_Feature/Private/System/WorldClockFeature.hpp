#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::System
{

	class WorldClockFeature
	{
	public:
		static void Init(Engine::ECS::EntityMgr& ecs);
		static void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);

		//
		//

		static void Init(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}