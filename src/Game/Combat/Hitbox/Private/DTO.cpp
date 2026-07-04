#include "Game/Combat/HitBox/Internal/DTO.hpp"
#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"

#include "Game/Combat/HitBox/Private/HitBoxBuilder.hpp"

namespace {

	using namespace Game::Combat::HitBox;

	void make_HitBoxDTO(HitBoxBuilder& builder)
	{
		HitBoxDTO default_obb2d;
		default_obb2d.name = "default_obb2D";
		default_obb2d.shape = "Obb2D";
		default_obb2d.angle = 0.f;
		default_obb2d.offset = { 0.f, 1.0f };		// (横, 縦)
		default_obb2d.halfExtents = { 1.0f, 1.0f };

		builder.add(default_obb2d);

		// 以下 随時形状追加
	}
}

namespace Game::Combat::HitBox
{
	void RegisterHitBoxDTO(HitBoxBuilder& builder)
	{
		make_HitBoxDTO(builder);
	}
}