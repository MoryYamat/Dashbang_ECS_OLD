#pragma once

#include "Engine/WorldSystem/Private/Core/World.hpp"
#include "Engine/WorldSystem/Private/Core/WorldAccess.hpp"

namespace Engine::WorldSystem::Core
{
	struct WorldCtx
	{
		World& world;
		WorldRead rw{ world };
		WorldWrite ww{ world };
	};
}