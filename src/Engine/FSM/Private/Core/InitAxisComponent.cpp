#include "Engine/FSM/Public/Core/InitAxisComponent.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/Analysis.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <string>
#include <cstdint>

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
	)
	{
		auto& cat = ctx.ww.GetResource<FSMCatalog>();
		auto& db = ctx.ww.GetResource<AxisRuntimeDB>();

		AxisRuntime* rt = db.get(std::string(axisName));
		if (!rt || !rt->canon) return false;

		const CanonicalAxis* ax = rt->canon;

		auto itFSM = ax->fsmIdxByName.find(std::string(fsmName));
		if (itFSM == ax->fsmIdxByName.end()) return false;

		const std::uint32_t fsmIdx = itFSM->second;

		if (fsmIdx >= ax->fsms.size() || fsmIdx >= rt->evals.size()) return false;

		outAxis = ax;
		outFSM = &ax->fsms[fsmIdx];
		outPlan = &rt->plan;
		outEval = &rt->evals[fsmIdx];
		return true;
	}
}