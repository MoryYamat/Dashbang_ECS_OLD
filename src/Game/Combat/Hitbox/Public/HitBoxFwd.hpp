#pragma once

#include <cstdint>
#include "Engine/ECS/Public/Entity.hpp"

namespace Game::Combat::HitBox
{
	template<typename T>
	struct StrongID
	{
		std::uint16_t v = UINT16_MAX;

		constexpr StrongID() = default;
		constexpr explicit StrongID(std::uint16_t id) : v(id) {};

		constexpr bool valid() const
		{
			return v != UINT16_MAX;
		}

		friend constexpr bool operator ==(StrongID, StrongID) = default;
	};

	struct HitBoxTag{};
	using HitBoxID = StrongID<HitBoxTag>;
	inline constexpr HitBoxID kInvalidHitBoxID{};


	struct HitBoxCatalog;
	class HitBoxBuilder;
}