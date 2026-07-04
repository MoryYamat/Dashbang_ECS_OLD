// camera datas
#pragma once

#include <glm/glm.hpp>

namespace Engine::ECS::Component::Camera
{
	struct CameraComponent
	{
		float fov = glm::radians(60.0f);
		float aspect;
		float nearClip = 0.1f;
		float farClip = 100.0f;

		float yaw = 0.0f;
		float pitch = 45.0f;
				
		glm::vec3 target = glm::vec3(0.0f);

		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

		glm::vec3 frontOffset = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec4 viewport = glm::vec4(1.0f);// viewport

		float moveSpeed = 5.0f;
		float mouseSensitivity = 0.1f;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
	};
}

