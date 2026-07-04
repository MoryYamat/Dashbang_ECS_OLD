#pragma once

#include "Engine/Physics/Public/Geometry2DTypes.hpp"


#include <glm/glm.hpp>

#include <cmath>
#include <variant>
#include <cassert>

namespace Engine::Component
{


	// Collider Data Structure
	struct Collider
	{
		//Engine::ECS::Component::Logic2D::ColliderType type;

		// パフォーマンスと安全性を高める方法を考えるまではVariantで
		Physics::ShapeVariant shape;

		Physics::ShapeKind getType() const
		{
			return std::visit([](auto&& arg) -> Physics::ShapeKind {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, Physics::Circle2D>) return Physics::ShapeKind::Circle2D;
				else if constexpr (std::is_same_v<T, Physics::Box2D>) return Physics::ShapeKind::Box2D;
				else if constexpr (std::is_same_v<T, Physics::Obb2D>) return Physics::ShapeKind::Obb2D;
				else return Physics::ShapeKind::None;
				}, shape);
		}

		// helper functions
		bool IsCircle2D() const { return std::holds_alternative<Physics::Circle2D>(shape); }
		bool IsBox2D() const { return std::holds_alternative<Physics::Box2D>(shape); }
		bool IsObb2D() const { return std::holds_alternative<Physics::Obb2D>(shape); }

		Physics::Circle2D& AsCircle2D() { return std::get<Physics::Circle2D>(shape); }
		Physics::Box2D& AsBox2D() { return std::get<Physics::Box2D>(shape); }
		Physics::Obb2D& AsObb2D() { return std::get<Physics::Obb2D>(shape); }

		const Physics::Circle2D& AsCircle2D() const
		{
			assert(IsCircle2D());
			return std::get<Physics::Circle2D>(shape);
		}
		const Physics::Box2D& AsBox2D() const
		{
			assert(IsBox2D());
			return std::get<Physics::Box2D>(shape);
		}
		const Physics::Obb2D& AsObb2D() const
		{
			assert(IsObb2D());// 開発時のみチェック
			return std::get<Physics::Obb2D>(shape);
		}

	};


	struct CollisionComponent
	{
		Collider collider;
		bool isStatic = true;
	};
}