#pragma once

// #include "Game/Input/Public/InputFwd.hpp"
#include "Game/Input/Public/InputActionComponent.hpp"
#include "Game/Combat/Skill/Public/CoreFwd.hpp"

#include <memory>
#include <cstddef>
#include <array>

namespace Game::Combat::Skill
{
	enum class SkillSlotIndex : std::uint8_t
	{
		Slot0,
		Slot1,
		Slot2,
		Slot3,
		COUNT,
	};

	struct SkillInputBindingComponent
	{
		void DefaultInit();

		std::unordered_map<Game::Input::InputAction, SkillSlotIndex> actionToSlot;
	};

	// Runtime
	struct SkillSlotComponent
	{
		static constexpr std::size_t MaxSlots = 4;
		std::array<SkillID, MaxSlots> slotSkills;
	};
}