#pragma once

#include <glm/glm.hpp>

namespace Engine::Component
{
	struct Velocity2DComponent
	{
		glm::vec2 velocity = glm::vec2(0.0f);
	};
}