#pragma once

#include <cstdint>
#include <unordered_map>


namespace Engine::FSM::Core
{
	template<typename T>
	struct StrongID
	{
		std::uint32_t v = UINT32_MAX;

		constexpr StrongID() = default;
		constexpr explicit StrongID(std::uint32_t id) : v(id) {};

		constexpr bool valid() const
		{
			return v != UINT32_MAX;
		}

		friend constexpr bool operator == (StrongID, StrongID) = default;
	};

	struct StateTag {}; using StateID = StrongID<StateTag>;
	struct CondTag {}; using CondID = StrongID<CondTag>;
	struct SlotTag {}; using SlotID = StrongID<SlotTag>;
	struct ProfileTag {}; using ProfileID = StrongID<ProfileTag>;
	struct FSMTag {}; using FSMID = StrongID<FSMTag>;
	struct AxisTag {}; using AxisID = StrongID<AxisTag>;

	inline constexpr StateID	kInvalidState{};
	inline constexpr CondID		kInvalidCond{};
	inline constexpr SlotID		kInvalidSlot{};
	inline constexpr ProfileID	kInvalidProfile{};
	inline constexpr FSMID		kInvalidFSM{};
	inline constexpr AxisID		kInvalidAxis{};
	inline constexpr std::uint32_t kInvalidID = UINT32_MAX;

	struct PerStateProfilePlan;
	struct PrecomputedEvalPlans;
	struct NextEvalSet;
	

	class FSMRegistry;
	struct CanonicalAxis;
	struct FSMCatalog;
	struct EnvAssemblerPlan;
	struct Decision;


	struct AxisInstance;

}