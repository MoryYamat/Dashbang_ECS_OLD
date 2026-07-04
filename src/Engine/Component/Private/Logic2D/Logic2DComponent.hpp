#pragma once

#include "Engine/Config/CanonicalDefaults.h"
#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"

#include <glm/glm.hpp>

namespace Engine::Component
{
	struct Logic2DTransformComponent
	{
		//position (x, z)
		glm::vec2 positionXZ = { 0.0f, 0.0f };

		// direction (radians)
		float rotation = CanonicalDefaults::kCanonicalRotationY;// radians!

		// Front and right vectors
		glm::vec2 front = CanonicalDefaults::kCanonicalForwardXZ;
		glm::vec2 right = CanonicalDefaults::kCanonicalRightXZ;

		// scale
		glm::vec2 scale = glm::vec2(1.0f);

		// Frontベクトルから回転を得る (radians)
		float GetRotationYFromFrontVector() const
		{
			return Engine::Math::Logic2D::CalcYawFromDirection(front);
		}

		// FrontベクトルからRightベクトルを得る
		void UpdateRightFromFront()
		{
			right = Engine::Math::Logic2D::CalcRightFromForward(front);
		}

		// 初期化時のみ　※ゲームループでは使わない
		void UpdateDirectionFromRotation()
		{
			front = glm::normalize(Engine::Math::Logic2D::CalcForwardFromYaw(rotation));
			right = Engine::Math::Logic2D::CalcRightFromYaw(rotation);
		}

	};
}