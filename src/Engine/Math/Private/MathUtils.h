// 汎用計算関数群

#pragma once

#include "Engine/Config/CanonicalDefaults.h"



#include <glm/glm.hpp>

#include <iostream>

// Define semantic transformations in terms of mathematical transformations
namespace Engine::Math
{
	// 度からラジアンへ変換する
	inline float DegreesToRadians(float deg) { return glm::radians(deg); }
	// ラジアンから度へ変換する
	inline float RadiansToDegrees(float rad) { return glm::degrees(rad); }

	// 標準的な2D回転（XY空間）
	inline glm::vec2 RotateVec2(const glm::vec2& vec, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * vec.x - s * vec.y,
			s * vec.x + c * vec.y
		);
	}

	// 回転行列ベース
	inline glm::mat2 RotationMatrix2D(float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::mat2(c, -s, s, c);
	}





}
