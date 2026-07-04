#pragma once

#include "Engine/Config/CanonicalDefaults.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Component
{
	struct TransformComponent
	{
		glm::vec3 position = glm::vec3(0.0f);
		// need to convert to a quaternion
		// degreesで保持し，内部の行列生成はglm::radians()で変換する
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);


		glm::mat4 toMatrix() const
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

			// 補正値
			constexpr float modelFacingOffsetDeg = 0.0f;

			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
			model = glm::rotate(model, glm::radians(rotation.y + modelFacingOffsetDeg), glm::vec3(0, 1, 0));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
			model = glm::scale(model, scale);
			return model;
		}
	};
}