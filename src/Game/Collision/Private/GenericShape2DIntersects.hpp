//

#pragma once

#include "Game/Collision/Private/GenericShape2D.hpp"

#include <variant>
#include <cmath>

#include <glm/glm.hpp>



namespace Game::Collision
{

	// 外部インターフェース
	bool Intersects(const GenericShape2D& a, const GenericShape2D& b);

	// 組み合わせ別の内部処理 (必要に応じてprivate namespace detailに)
	// 2D円と2D円の重なり判定
	bool Intersects_Circle_Circle(const Circle2D& a, const Circle2D& b);

	// 2D円と扇形の重なり判定
	bool Intersects_Circle_Sector(const Circle2D& circle, const Sector2D& sector);

	// 2D円と矩形の重なり判定
	bool Intersects_Circle_Obb(const Circle2D& circle, const Obb2D& obb);

	// 扇形と扇形の重なり判定
	bool Intersects_Sector_Sector(const Sector2D& a, const Sector2D& b);

	// 扇形と矩形の重なり判定
	bool Intersects_Sector_Obb(const Sector2D& sector, const Obb2D& obb);

	// 矩形と矩形の重なり判定
	bool Intersects_Obb_Obb(const Obb2D& a, const Obb2D& b);



	// optional
}