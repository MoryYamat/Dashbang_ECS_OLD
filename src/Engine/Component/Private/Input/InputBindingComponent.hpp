#pragma once

#include "Engine/ECS/Public/Entity.hpp"

namespace Engine::Component
{
	struct InputBindingComponent
	{
		Engine::ECS::Core::Entity controllingEntity;
	};
}