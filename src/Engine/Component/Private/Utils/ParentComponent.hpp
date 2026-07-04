#pragma once

#include "Engine/ECS/Public/Entity.hpp"

namespace Engine::Component
{
	struct ParentComponent
	{
		Engine::ECS::Core::Entity parent;
	};
}