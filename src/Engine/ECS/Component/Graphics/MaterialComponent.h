// models Material datas

#pragma once

#include <glm/glm.hpp>

namespace Engine::ECS::Component::Graphics
{
	struct MaterialComponent
	{
		glm::vec3 baseColor = glm::vec3(1.0f);// RGB
	};
}

