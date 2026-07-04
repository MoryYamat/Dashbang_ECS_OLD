#pragma once

#include "Engine/ECS/Entity.h"



namespace Engine::ECS::Component::Input
{
	struct InputBindingComponent
	{
		Engine::ECS::Entity controllingEntity;
	};
}