#pragma once

#include <cstdint>
#include <functional>

namespace Game::Combat::Skill
{
	template<typename T>
	struct StrongID
	{
		std::uint16_t v = UINT16_MAX;

		constexpr StrongID() = default;
		constexpr explicit StrongID(std::uint16_t value) : v(value) {};

		constexpr bool valid() const
		{
			return v != UINT16_MAX;
		}

		friend constexpr bool operator == (StrongID, StrongID) = default;
	};
	struct SkillTag {}; using  SkillID = StrongID<SkillTag>;
	// struct EffectSlotTag {}; using EffectSlotID = StrongID<EffectSlotTag>;

	
	inline constexpr SkillID kInvalidSkillID{};
	// inline constexpr EffectSlotID kInvalidEffectSlotID{};
	
	struct CanonicalSkillData;
	struct SkillRuntimeComp;
	struct SkillCatalog;
	class SkillCatalogBuilder;
	struct SkillLogicCommandBuffer;

	struct SkillSlotLoadoutDTO;
	struct SkillSlotComponent;

	struct SkillRequest;
}

namespace std {
	template<typename T>
	struct hash<Game::Combat::Skill::StrongID<T>>
	{
		size_t operator()(const Game::Combat::Skill::StrongID<T>& id) const noexcept
		{
			return std::hash<std::uint16_t>{}(id.v);
		}
	};
}


namespace Game::Combat::Skill::Binding
{
	class SkillBindingBuilder;
}