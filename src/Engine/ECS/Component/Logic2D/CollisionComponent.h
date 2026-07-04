// Collision Component 

#pragma once

#include "Engine/ECS/Component/Logic2D/ColliderType.h"



namespace Engine::ECS::Component::Logic2D
{
	struct CollisionComponent
	{
		Engine::ECS::Component::Logic2D::Collider collider;
		bool isStatic = true;
	};
}
