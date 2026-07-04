// Specify the camera's target
// 
#pragma once

#include <cstdint>

#include "Engine/ECS/Entity.h"

#include <glm/glm.hpp>

namespace Engine::ECS::Component::Camera
{
	struct FollowCameraComponent
	{
		// target's entity id 
		Entity targetEntity = Entity::INVALID;
		glm::vec3 offset = { 0.0f, 10.0f, -10.0f };
	};
}
