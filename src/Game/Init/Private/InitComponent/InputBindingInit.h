//

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Init::Input
{
	void InputBindingInitializationSystem(Engine::ECS::EntityMgr& ecs);

	void InputBindingInitializationSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}