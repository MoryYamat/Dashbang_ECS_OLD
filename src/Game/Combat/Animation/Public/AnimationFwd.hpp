#pragma once

#include <cstdint>
#include <functional>

#include "Engine/ECS/Public/Entity.hpp"


namespace Game::Combat::Animation
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

	struct AnimTag{};
	using AnimID = StrongID<AnimTag>;
	inline constexpr AnimID kInvalidAnimID{};

	struct AnimCatalog;	
	class AnimBuilder;

}

namespace std
{
	template<typename T>
	struct hash<Game::Combat::Animation::StrongID<T>>
	{
		size_t operator() (const Game::Combat::Animation::StrongID<T>& id) const noexcept
		{
			return std::hash<std::uint16_t>{}(id.v);
		}
	};
}