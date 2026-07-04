// (Child) entities have a hierarchical structure by having a parent entity ID.

#pragma once

#include "Engine/ECS/Entity.h"

namespace Engine::ECS::Component::Utils
{
	struct ParentComponent
	{
		Entity parent;
	};
}

