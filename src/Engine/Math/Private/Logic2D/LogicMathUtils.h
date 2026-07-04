// 論理用計算関数群


#pragma once
#include "Engine/Config/CanonicalDefaults.h"

// 

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cmath>
#include <iostream>

namespace Engine::Math::Logic2D
{
	// ゲームロジック用（+Zが正面） rotationY = 0 -> return = (0, 1) rotationY = pi/2 -> return = (1, 0)
	inline glm::vec2 RotateVec2FromZForward(const glm::vec2& vec, float radians)
	{
		float c = glm::cos(radians);
		float s = glm::sin(radians);
		return glm::vec2(
			c * vec.x + s * vec.y,
			-s * vec.x + c * vec.y
		);
	}

	//

	// +Zが前方の場合のradiansだけ回転したベクトルを得る
	inline glm::vec2 CalcForwardFromYaw(float radians)
	{
		return RotateVec2FromZForward(CanonicalDefaults::kLocalForwardXZ, radians);
	}

	// +Zが前方の場合のradinasだけ回転したFrontベクトルに対するRightベクトルを得る
	inline glm::vec2 CalcRightFromYaw(float radians)
	{
		return RotateVec2FromZForward(CanonicalDefaults::kCanonicalForwardXZ, radians + glm::half_pi<float>());
	}

	// [-pi, pi]を[0, 2pi]に正規化
	inline float NormalizeAnglePositive(float rad)
	{
		float constexpr twoPi = 2.0f * glm::pi<float>();
		float result = std::fmod(rad, twoPi);
		if (result < 0.0f)
			result += twoPi;
		return result;
	}

	// 回転原点を定義
	// 右手系(+Zが正面(0.0°) : ベクトルの回転をラジアンで返す
	inline float CalcYawFromDirection(glm::vec2 vecXZ)
	{
		// もう少し良い分岐方法を考える
		if (glm::length(vecXZ) > 0.0001f)
		{
			// atan2 -> [-pi, pi] 
			// return std::atan2(vecXZ.y, vecXZ.x);// +X軸から+Z軸
			// return std::atan2(-vecXZ.y, vecXZ.x);// +X軸から-Z軸
			// return std::atan2(vecXZ.y, -vecXZ.x);// -X軸から+Z軸
			// return std::atan2(-vecXZ.y, -vecXZ.x);// -X軸から-Z軸

			// [0, 2pi] に補正
			// return NormalizeAnglePositive(std::atan2(-vecXZ.x, -vecXZ.y));// -Z軸から-X軸
			// return std::atan2(-vecXZ.x, vecXZ.y);// Z軸から-X軸
			// return std::atan2(vecXZ.x, -vecXZ.y);// -Z軸から+X軸
			return NormalizeAnglePositive(std::atan2(vecXZ.x, vecXZ.y));// +Z軸から+X軸
		}
		else
		{
			return CanonicalDefaults::kCanonicalRotationY;
		}
	}


	// 右手系(反時計回りが正)において，あるベクトルに対する右ベクトルを返す
	inline glm::vec2 CalcRightFromForward(glm::vec2 front)
	{
		return glm::vec2(-front.y, front.x);
		//return glm::vec2(front.y, front.x);
	}

	// [0, 2pi) の角度を [-pi, pi]にマップ
	inline float ToSignedPi(float rad)
	{
		constexpr float twoPi = 2.0f * glm::pi<float>();
		float a = std::fmod(rad + glm::pi<float>(), twoPi);
		if (a < 0.0f) a += twoPi;
		return a - glm::pi<float>();
	}

}

namespace Engine::Math::Logic2D
{
	namespace Vector
	{
		inline float SquaredLength(const glm::vec2& vec)
		{
			return vec.x * vec.x + vec.y * vec.y;
		}
	}
}

namespace Engine::Math::Logic2D
{
	namespace Transform
	{
		// ローカルオフセットをワールド空間へ変換(位置ベクトル)
		inline glm::vec2 ApplyLocalOffset(
			const glm::vec2& localOffset,
			const glm::vec2& positionXZ,
			float rotationY = 0.0f,
			glm::vec2 scale = glm::vec2(1.0f)
		)
		{
			glm::vec2 rotated = RotateVec2FromZForward(localOffset, rotationY);
			return positionXZ + rotated * scale;
		}

		// ローカル方向ベクトルをワールド空間へ変換（normalize推奨）
		inline glm::vec2 TransformDirection(
			const glm::vec2& localDirection,
			float rotationY = 0.0f// ローカル回転
		)
		{
			return RotateVec2FromZForward(localDirection, rotationY);
		}
	}
}