#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/FSM/Public/Core/AxisComponent.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"

#include <string_view>
#include <cassert>


namespace Engine::FSM::Core
{

	bool ResolveAxisAndFSM
	(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		std::string_view axisName,
		std::string_view fsmName,
		const CanonicalAxis*& outAxis,
		const CanonicalFSM*& outFSM,
		const EnvAssemblerPlan*& outPlan,
		const PrecomputedEvalPlans*& outEval
	);

	template<typename AxisTag, typename ReaderT>
	void InitAxisComponentFromResolved
	(
		AxisComponent<AxisTag, ReaderT>& comp,
		const CanonicalAxis& ax,
		const CanonicalFSM& fsm,
		const EnvAssemblerPlan& plan,
		const PrecomputedEvalPlans& eval
	)
	{
		comp.inst.init(
			ax,
			fsm,
			plan,
			eval,
			comp.initState,
			comp.initProfile
		);
	}

	// ★ ここが「ひな型から初期化」する共通関数
	template<typename AxisTag, typename ReaderT>
	bool InitAxisComponent
	(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		AxisComponent<AxisTag, ReaderT>& comp,
		std::string_view fsmNameOverride = {}
	)
	{
		using namespace Engine::WorldSystem::Core;

		const CanonicalAxis* ax = nullptr;
		const CanonicalFSM* fsm = nullptr;
		const EnvAssemblerPlan* plan = nullptr;
		const PrecomputedEvalPlans* eval = nullptr;

		const std::string_view axisName = AxisTraits<AxisTag>::AxisName();
		const std::string_view fsmName =
			!fsmNameOverride.empty()
			? fsmNameOverride
			: AxisTraits<AxisTag>::DefaultFSMName();

		if (!ResolveAxisAndFSM(ctx, axisName, fsmName, ax, fsm, plan, eval))
		{
			return false;
		}

		InitAxisComponentFromResolved(comp, *ax, *fsm, *plan, *eval);
		return true;
	}

}