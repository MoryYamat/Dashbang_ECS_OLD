#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"


#pragma once

#include "Engine/ECS/Public/ECSFwd.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/AxisInstance.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <cassert>
#include <string_view>
#include <iostream>

// (state/cond/slot/prof/fsm)の(定義)文字列からを正規化済みIDを取得するヘルパー関数
namespace Engine::FSM::Core::AxisLookup
{
	AxisID FindAxisIdxByName(const FSMCatalog& cat, std::string_view name) noexcept
	{
		for (const auto& axis : cat.axes)
		{
			if (axis.axisName == name)
			{
				return axis.axis;
			}
		}
		return kInvalidAxis;
	}

	std::uint32_t FindStateIdx(const CanonicalAxis& ax, std::string_view name) noexcept
	{
		if (auto it = ax.stateIdxByName.find(std::string(name)); it != ax.stateIdxByName.end())
			return it->second;

		assert(false && "AxisLookup::State: unknown state name");
		return kInvalid;
	}

	std::uint32_t FindCondIdx(const CanonicalAxis& ax, std::string_view name) noexcept
	{
		if (auto it = ax.condIdxByName.find(std::string(name)); it != ax.condIdxByName.end())
			return it->second;
		assert(false && "AxisLookup::Cond: unknown cond name");
		return kInvalid;
	}

	std::uint32_t FindSlotIdx(const CanonicalAxis& ax, std::string_view name) noexcept
	{
		if (auto it = ax.slotIdxByName.find(std::string(name)); it != ax.slotIdxByName.end())
			return it->second;
		assert(false && "AxisLookup::Slot: unknown slot name");
		return kInvalid;
	}

	std::uint32_t FindProfileIdx(const CanonicalAxis& ax, std::string_view name) noexcept
	{
		if (auto it = ax.profileIdxByName.find(std::string(name)); it != ax.profileIdxByName.end())
			return it->second;
		assert(false && "AxisLookup::Profile: unknown profile name");
		return kInvalid;
	}

	std::uint32_t FindFSMIdx(const CanonicalAxis& ax, std::string_view name) noexcept
	{
		if (auto it = ax.fsmIdxByName.find(std::string(name)); it != ax.fsmIdxByName.end())
			return it->second;
		assert(false && "AxisLookup::FSM: unknown fsm name");
		return kInvalid;
	}
}

// 逆に ID から文字列を取得するヘルパ関数集合
namespace Engine::FSM::Core::AxisLookup
{
	std::string_view GetStateName(const AxisInstance& inst, std::uint32_t localIdx)
	{
		const auto& ax = *inst.ax;
		const auto& fsm = *inst.fsm;

		// local -> global
		const StateID gid = fsm.local2GlobalState[localIdx];// StateID{idx};
		const std::uint32_t idx = gid.v;
		return ax.stateOrder[idx];
	}

	std::string_view GetSlotName(const AxisInstance& inst, std::uint32_t localIdx)
	{
		const auto& ax = *inst.ax;
		const auto& fsm = *inst.fsm;

		// local -> global
		const SlotID gid = fsm.local2GlobalSlot[localIdx];// StateID{idx};
		const std::uint32_t idx = gid.v;
		return ax.slotOrder[idx];
	}

	std::string_view GetProfileName(const AxisInstance& inst, std::uint32_t localIdx)
	{
		const auto& ax = *inst.ax;
		const auto& fsm = *inst.fsm;

		// local -> global
		const ProfileID gid = fsm.local2GlobalProfile[localIdx];// StateID{idx};
		const std::uint32_t idx = gid.v;
		return ax.profileOrder[idx];
	}
}

namespace Engine::FSM::Core
{

	bool IsState(const AxisInstance& inst,
		std::uint32_t localIdx,
		std::string_view name)
	{
		auto sv = AxisLookup::GetStateName(inst, localIdx);
		return sv == name;
	}
}

namespace Engine::FSM::Debug
{

	void PrintFSMInfo(Engine::ECS::Core::Entity e, const Engine::FSM::Core::AxisInstance& inst, Engine::FSM::Core::Decision& d)
	{
		using namespace Engine::FSM::Core;
		using namespace Engine::FSM::Core::AxisLookup;
		const auto fromName = GetStateName(inst, d.from);
		const auto toName = GetStateName(inst, d.to);
		const auto slotName = (d.slot != UINT32_MAX) ? GetSlotName(inst, d.slot) : std::string_view("<none>");

		std::cerr << "[FSM Transition Info]: e:'" << e.id
			<< "' axis:'" << inst.ax->axisName.c_str()
			<< "' fsm:'" << inst.fsm->name.c_str()
			<< "' s:'" << std::string(fromName).c_str()
			<< "' -> '" << std::string(toName).c_str()
			<< "' via slot:'" << std::string(slotName).c_str()
			<< "' (prio:" << d.prio << "\n";
	}
}