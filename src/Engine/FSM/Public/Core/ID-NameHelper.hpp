#pragma once

#include "Engine/ECS/Public/ECSFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
// #include "Types.hpp"
// #include "AxisInstance.hpp"
#include <cassert>
#include <string_view>
#include <iostream>

// (state/cond/slot/prof/fsm)の(定義)文字列からを正規化済みIDを取得するヘルパー関数
namespace Engine::FSM::Core::AxisLookup
{

	static constexpr std::uint32_t kInvalid = UINT32_MAX;

	AxisID FindAxisIdxByName(const FSMCatalog& cat, std::string_view name) noexcept;

	std::uint32_t FindStateIdx(const CanonicalAxis& ax, std::string_view name) noexcept;
	
	std::uint32_t FindCondIdx(const CanonicalAxis& ax, std::string_view name) noexcept;

	std::uint32_t FindSlotIdx(const CanonicalAxis& ax, std::string_view name) noexcept;

	std::uint32_t FindProfileIdx(const CanonicalAxis& ax, std::string_view name) noexcept;

	std::uint32_t FindFSMIdx(const CanonicalAxis& ax, std::string_view name) noexcept;
}

// 逆に ID から文字列を取得するヘルパ関数集合
namespace Engine::FSM::Core::AxisLookup
{
	std::string_view GetStateName(const AxisInstance& inst, std::uint32_t localIdx);

	std::string_view GetSlotName(const AxisInstance& inst, std::uint32_t localIdx);

	std::string_view GetProfileName(const AxisInstance& inst, std::uint32_t localIdx);
}

namespace Engine::FSM::Core
{
	bool IsState(const AxisInstance& inst,
		std::uint32_t localIdx,
		std::string_view name);
}

