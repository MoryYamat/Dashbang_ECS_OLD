// Intent -> Velocity -> Position

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Character
{
	class MovementFeature
	{
	public:

		// intent
		static void UpdateMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx);

		// expression
		static void UpdateVelocity(Engine::WorldSystem::Core::WorldCtx& ctx);

		// logic
		static void UpdateLogicFacing(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateLogicPosition(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}
