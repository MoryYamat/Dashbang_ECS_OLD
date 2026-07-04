#pragma once

#include <glm/glm.hpp>

namespace Engine::Component
{
	struct AnalogInputComponent
	{
		// マウスカーソル
		glm::vec2 cursorLogicPositionXZ = glm::vec2(0.0f);
		glm::vec2 cursorDelta = glm::vec2(0.0f);
		float scrollDelta = 0.0f;

		bool isVisible = true;
		bool isHoveringUI = false;
		float debugDrawSize = 0.02f;

		// ゲームパッドスティック
		glm::vec2 leftStick = glm::vec2(0.0f);
		glm::vec2 rightStick = glm::vec2(0.0f);

	};
}