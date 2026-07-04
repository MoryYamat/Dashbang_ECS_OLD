// 描画用計算関数群


#pragma once

#include "Engine/Config/CanonicalDefaults.h"



#include <glm/glm.hpp>

#include <iostream>

namespace RenderMathUtils
{

	// 描画・モデル基準用（+Zが正面） rotation = 0 -> front = (0, +1)
	inline glm::vec2 RotateVec2_FromPositiveZ(const glm::vec2& vec, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * vec.x - s * vec.y,
			s * vec.x + c * vec.y
		);
	}
}