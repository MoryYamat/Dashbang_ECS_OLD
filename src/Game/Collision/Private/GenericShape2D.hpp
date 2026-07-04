#pragma once

#include <variant>
#include <glm/vec2.hpp>

namespace Game::Collision
{
	//円
	struct Circle2D
	{
		glm::vec2 center;
		float radius;
	};

	// 長方形
	struct Box2D
	{
		glm::vec2 center;
		glm::vec2 halfExtents;
	};

	// OBB
	struct Obb2D
	{
		glm::vec2 center;
		glm::vec2 halfExtents;
		glm::vec2 axisX;
		glm::vec2 axisZ;
	};

	// 扇方
	struct Sector2D
	{
		glm::vec2 center;
		glm::vec2 direction;
		float angle; // rad
		float radius;
	};

	using GenericShape2D = std::variant<
		std::monostate,
		Circle2D,
		Box2D,
		Obb2D,
		Sector2D
	>;

	inline const char* GetShapeName(const GenericShape2D& shape)
	{
		return std::visit([](auto&& s) -> const char* {
			using T = std::decay_t<decltype(s)>;
			if constexpr (std::is_same_v<T, Circle2D>) return "Circle2D";
			else if constexpr (std::is_same_v<T, Box2D>) return "Box2D";
			else if constexpr (std::is_same_v<T, Obb2D>) return "Obb2D";
			else if constexpr (std::is_same_v<T, Sector2D>) return "Sector2D";
			else return "None";
			}, shape);
	}

	inline GenericShape2D FromCircle(const glm::vec2& center, float radius)
	{
		return GenericShape2D{ Circle2D{ center, radius } };
	}

	inline GenericShape2D FromSector(const glm::vec2& center, const glm::vec2& dir, float angle, float radius)
	{
		return GenericShape2D{ Sector2D{ center, dir, angle, radius } };
	}

	inline GenericShape2D FromObb(const glm::vec2& center, const glm::vec2& halfExtents,
		const glm::vec2& axisX, const glm::vec2& axisZ)
	{
		return GenericShape2D{ Obb2D{ center, halfExtents, axisX, axisZ } };
	}
}