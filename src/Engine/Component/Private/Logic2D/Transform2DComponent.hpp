#pragma once

#include "Engine/Config/CanonicalDefaults.h"
#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"

#include <glm/gtx/rotate_vector.hpp>
#include <GLM/glm.hpp>

namespace Engine::Component
{
	struct Transform2DComponent
	{
		glm::vec2 positionXZ = { 0.0f, 0.0f };
		float rotationY = 0.0f; // radians
		float scale = 1.0f; // uniform

		glm::vec2 front = CanonicalDefaults::kLocalForwardXZ;
		glm::vec2 right = Engine::Math::Logic2D::CalcRightFromForward(front);

		// front (radians前提)
		glm::vec2 GetFrontXZ() const
		{
			return Engine::Math::Logic2D::CalcForwardFromYaw(rotationY);// +Z基準
		}

		// right (radians前提)
		glm::vec2 GetRightXZ() const
		{
			return Engine::Math::Logic2D::CalcRightFromYaw(rotationY);// +Z基準
		}

		// frontに合わせて更新
		void UpdateRightFromFront()
		{
			right = Engine::Math::Logic2D::CalcRightFromForward(front);
		}

	};
}