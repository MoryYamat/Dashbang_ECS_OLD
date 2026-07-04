#pragma once

#include <cstdint>
#include <cmath>
#include <variant>
#include <glm/glm.hpp>


namespace Engine::Physics
{
	enum class ShapeKind : std::uint8_t
	{
		None,
		Circle2D,
		Box2D,
		Obb2D,
		// Sector2D, ... 
	};

	struct Circle2D
	{
		// ローカルオフセット
		glm::vec2 center;
		float radius;
	};

	// 2D正方形
	struct Box2D
	{
		// ローカルオフセット
		glm::vec2 center;
		glm::vec2 halfExtents;
	};

	// 回転あり境界ボックス
	struct Obb2D
	{
		// ローカルオフセット
		glm::vec2 center;//
		glm::vec2 halfExtents; // (width/2, height/2)
		glm::vec2 axisX; // X軸方向の単位ベクトル
		glm::vec2 axisZ;// axisに直交

		// 点が含まれるかどうかを判定
		bool contains(const glm::vec2& pointXZ) const
		{
			// 相対ベクトル
			glm::vec2 d = pointXZ - center;
			float dx = glm::dot(d, axisX);// ローカルx軸への投影
			float dz = glm::dot(d, axisZ);// ローカルz軸への投影

			// 投影の絶対値とモデル半径(XZそれぞれを)比較して判定
			return std::abs(dx) <= halfExtents.x && std::abs(dz) <= halfExtents.y;
		}
	};

	using ShapeVariant = std::variant<std::monostate, Circle2D, Box2D, Obb2D>;
}