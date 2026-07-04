#pragma once

#include <cstdint>

namespace Engine::ECS::Core
{
	struct Entity
	{
		std::uint32_t id{ 0 };
		
		constexpr Entity() = default;
		constexpr Entity(uint32_t id) : id(id) {}


		constexpr bool operator==(Entity o) const noexcept { return id == o.id; }
		constexpr bool operator!=(Entity o) const noexcept { return id != o.id; }
		constexpr bool IsValid() const noexcept { return id != 0; }

		
	};
	inline static constexpr Entity INVALID{ 0 };
}