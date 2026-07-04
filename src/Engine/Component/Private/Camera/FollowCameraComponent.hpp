#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include <glm/glm.hpp>

namespace Engine::Component
{
	struct FollowCameraComponent
	{
		// target's entity id 
		Engine::ECS::Core::Entity targetEntity = Engine::ECS::Core::INVALID;
		glm::vec3 offset = { 0.0f, 10.0f, -10.0f };
	};
}