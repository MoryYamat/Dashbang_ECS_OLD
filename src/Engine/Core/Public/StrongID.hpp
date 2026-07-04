#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <functional>

namespace Engine::Core
{
	template<typename T, typename Rep = std::uint32_t>
	class StrongID
	{
	public:
		using rep_type = Rep;

		// return invalid value
		static constexpr StrongID Invalid() noexcept
		{
			return StrongID{ invalid_value() };
		}

		constexpr StrongID() noexcept : value_(invalid_value()) {}
		explicit constexpr StrongID(Rep v) noexcept : value_(v) {}

		// return raw value
		constexpr Rep value() const noexcept { return value_; }

		// Check if the value is invalid
		constexpr bool is_valid() const noexcept { return value_ != invalid_value(); }

		friend constexpr bool operator==(StrongID a, StrongID b) noexcept { return a.value_ == b.value_; }
		friend constexpr bool operator!=(StrongID a, StrongID b) noexcept { return !(a == b); }
		friend constexpr bool operator<(StrongID a, StrongID b) noexcept { return a.value_ < b.value_; }

	private:

		static consteval Rep invalid_value()
		{
			return std::numeric_limits<Rep>::max();
		}

		Rep value_;
	};

	struct StrongIDHasher
	{
		template<typename T, typename Rep>
		std::size_t operator()(const StrongID<T, Rep>& id) const noexcept
		{
			return std::hash<Rep>{}(id.value());
		}
	};
}

