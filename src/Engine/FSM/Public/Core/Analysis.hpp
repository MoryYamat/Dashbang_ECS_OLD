#pragma once

#include <vector>
#include <span>
#include <cstdint>

namespace Engine::FSM::Core
{

	struct PerStateProfilePlan
	{
		std::vector<std::uint32_t> candidateSlots;		// 出次数>0 のslotLocal集合
		std::vector<std::uint32_t> condBits;			// 必須CondBitのユニーク集合(CondID.v)
		std::vector<std::uint32_t> condBitOfSlot;		// size = numSlots, 未設定はUINT32_MAX
	};

	struct PrecomputedEvalPlans
	{
		// [stateLocal][profileLocal]
		std::vector<std::vector<PerStateProfilePlan>> table;
	};

	struct NextEvalSet
	{
		std::span<const std::uint32_t> slots;		// candidatesSlots
		std::span<const std::uint32_t> condBits;	// 必須bit集合(ユニーク)
	};

	struct CanonicalFSM;
	PrecomputedEvalPlans BuildEvalPlans(const CanonicalFSM& f);

	PerStateProfilePlan BuildPlanFor(
		const CanonicalFSM& f,
		std::uint32_t stateLocal,
		std::uint32_t profileLocal);

	NextEvalSet GetNextEvalSet(
		const PrecomputedEvalPlans& ps,
		std::uint32_t stateLocal,
		std::uint32_t profileLocal
	);
}