#pragma once


#include "Engine/Math/Public/MathAPI.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine::Graphics
{
	[[nodiscard]] inline glm::mat4 ToGlmMat4(const Engine::Math::TRS& t) noexcept
	{
		using ::Engine::Math::Vec3f;
		using ::Engine::Math::Quatf;

		// TRS -> glm
		const glm::vec3 tr = glm::vec3(t.translation.x, t.translation.y, t.translation.z);
		const glm::quat rq = glm::quat(t.rotation.w, t.rotation.x, t.rotation.y, t.rotation.z);
		const glm::vec3 sc = glm::vec3(t.scale.x, t.scale.y, t.scale.z);

		// çáê¨ T*R*S
		const glm::mat4 Tm = glm::translate(glm::mat4(1.0f), tr);
		const glm::mat4 Rm = glm::mat4_cast(rq);
		const glm::mat4 Sm = glm::scale(glm::mat4(1.0f), sc);

		return Tm * Rm * Sm;
	}
}