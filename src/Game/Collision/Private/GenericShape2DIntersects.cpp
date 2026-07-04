#include "GenericShape2DIntersects.hpp"

#include "Engine/Physics/Private/Logic2D/DetectionFunctions.h"

#include <iostream>



// 静的関数ディスパッチ => 動的関数ディスパッチへ移行することを検討するべき
bool Game::Collision::Intersects(const GenericShape2D& a, const GenericShape2D& b)
{
	return std::visit([](auto&& lhs, auto&& rhs) -> bool {
		using L = std::decay_t<decltype(lhs)>;
		using R = std::decay_t<decltype(rhs)>;

		if constexpr (std::is_same_v<L, std::monostate> || std::is_same_v<R, std::monostate>) {
			return false;
		}

		if constexpr (std::is_same_v<L, Circle2D> && std::is_same_v<R, Circle2D>) {
			return Intersects_Circle_Circle(lhs, rhs);
		}
		else if constexpr (std::is_same_v<L, Circle2D> && std::is_same_v<R, Sector2D>) {
			return Intersects_Circle_Sector(lhs, rhs);
		}
		else if constexpr (std::is_same_v<L, Sector2D> && std::is_same_v<R, Circle2D>) {
			return Intersects_Circle_Sector(rhs, lhs); // 引数の順番を入れ替える
		}
		else if constexpr (std::is_same_v<L, Circle2D> && std::is_same_v<R, Obb2D>) {
			return Intersects_Circle_Obb(lhs, rhs);
		}
		else if constexpr (std::is_same_v<L, Obb2D> && std::is_same_v<R, Circle2D>) {
			return Intersects_Circle_Obb(rhs, lhs); // 引数の順番を入れ替える
		}
		else if constexpr (std::is_same_v<L, Sector2D> && std::is_same_v<R, Sector2D>) {
			return Intersects_Sector_Sector(lhs, rhs);
		}
		else if constexpr (std::is_same_v<L, Sector2D> && std::is_same_v<R, Obb2D>) {
			return Intersects_Sector_Obb(lhs, rhs);
		}
		else if constexpr (std::is_same_v<L, Obb2D> && std::is_same_v<R, Sector2D>) {
			return Intersects_Sector_Obb(rhs, lhs); // 順序反転
		}
		else if constexpr (std::is_same_v<L, Obb2D> && std::is_same_v<R, Obb2D>) {
			return Intersects_Obb_Obb(lhs, rhs);
		}
		else {
			std::cout << "[IntersectsGenericShape2D.cpp]: No handler for ("
				<< typeid(L).name() << ", " << typeid(R).name() << ")\n";
			return false;
		}
		}, a, b);

		//// 無効な形状(monostate)ならfalse
		//if constexpr (std::is_same_v<L, std::monostate> || std::is_same_v<R, std::monostate>)
		//	return false;

		//// Circle - Circle
		//else if constexpr (std::is_same_v<L, Circle2D> && std::is_same_v<R, Circle2D>)
		//	return Intersects_Circle_Circle(lhs, rhs);

		//// Circle - Sector
		//else if constexpr (std::is_same_v<L, Circle2D> && std::is_same_v<R, Sector2D>)
		//	return Intersects_Circle_Sector(lhs, rhs);

		//// Circle - Obb		
		//else if constexpr (std::is_same_v<L, Circle2D> && std::is_same_v<R, Obb2D>)
		//	return Intersects_Circle_Obb(lhs, rhs);

		//// Sector - Sector
		//else if constexpr (std::is_same_v<L, Sector2D> && std::is_same_v<R, Sector2D>)
		//	return Intersects_Sector_Sector(lhs, rhs);

		//// Sector - Obb
		//else if constexpr (std::is_same_v<L, Sector2D> && std::is_same_v<R, Obb2D>)
		//	return Intersects_Sector_Obb(lhs, rhs);

		//// Obb - Obb
		//else if constexpr (std::is_same_v<L, Obb2D> && std::is_same_v<R, Obb2D>)
		//	return Intersects_Obb_Obb(lhs, rhs);

		//// ほかの判定も順次追加

		//else
		//	std::cout << "[IntersectsGenericShape2D.cpp(Intersects)]: There is no corresponding shape pair.\n";
		//	return false;

		//}, a, b);
}

bool Game::Collision::Intersects_Circle_Circle(const Circle2D& a, const Circle2D& b)
{
	return Engine::Physics::Logic2D::Collision::intersects_Circle2D_Circle2D(a.center, a.radius, b.center, b.radius);
}

bool Game::Collision::Intersects_Circle_Sector(const Circle2D& circle, const Sector2D& sector)
{
	return Engine::Physics::Logic2D::Collision::Intersects_Circle2D_Sector2D(circle.center, circle.radius,
		sector.center, sector.direction, sector.angle, sector.radius);
}

bool Game::Collision::Intersects_Circle_Obb(const Circle2D& circle, const Obb2D& obb)
{
	return Engine::Physics::Logic2D::Collision::Intersects_Circle2D_Obb2D(circle.center, circle.radius,
		obb.center, obb.halfExtents, obb.axisX, obb.axisZ
		);
}

bool Game::Collision::Intersects_Sector_Sector(const Sector2D& a, const Sector2D& b)
{
	return Engine::Physics::Logic2D::Collision::Intersects_Sector2D_Sector2D(a.center, a.direction, a.angle, a.radius,
		b.center, b.direction, b.angle, b.radius);
}

bool Game::Collision::Intersects_Sector_Obb(const Sector2D& sector, const Obb2D& obb)
{
	return Engine::Physics::Logic2D::Collision::Intersects_Sector2D_Obb2D(sector.center, sector.direction, sector.angle, sector.radius,
		obb.center, obb.halfExtents, obb.axisX, obb.axisZ
	);
}

bool Game::Collision::Intersects_Obb_Obb(const Obb2D& a, const Obb2D& b)
{
	return Engine::Physics::Logic2D::Collision::Intersects_Obb2D_Obb2D(a.center, a.halfExtents, a.axisX, a.axisZ,
		b.center, b.halfExtents, b.axisX, b.axisZ);
}
