#include "Game/Character/FSM/Public/SkillAxisApi.hpp"
#include "Game/Character/FSM/Public/SkillTypes.hpp"
#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"

#include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Time/Private/WorldClock.hpp"


#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"
#include "Game/Combat/SKill/Public/CoreAPI.hpp"

#include "Engine/Debug/Public/ProfilingAPI.hpp"

#include <iostream>

namespace
{
	using namespace Engine::WorldSystem::Query;
	using namespace Game::Character::FSM::Skill;

	void BuildSkillPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, SkillPipeline& out)
	{
		out.clear();
		auto ents = ViewWhere(ctx.rw, All<SkillAxisComp, SkillStateComp>{});

		for (const auto& e : ents)
		{
			auto* axis = ctx.ww.TryGet<SkillAxisComp>(e);
			auto* state = ctx.ww.TryGet<SkillStateComp>(e);


			out.push_back(SkillPipelineEntry{
					.e = e,
					.axis = axis,
					.state = state
				});
		}
	}
}

namespace Game::Character::FSM::Skill
{
	using namespace Engine::WorldSystem::Query;


	void SkillEnvSystem::Update(const std::span<SkillPipelineEntry> ents, const float dt)
	{
		const auto& condTable = ctx_.rw.GetResource<SkillCondTable>();

		for (auto& entry : ents)
		{
			auto& inst = entry.axis->inst;
			auto* rt = ctx_.ww.TryGet<Game::Combat::Skill::SkillRuntimeComp>(entry.e);
			if (!rt)continue;

			inst.beginFrame(dt);

			// inst.setBase
			// --- Triggered bit ---
			if (condTable.Triggered.valid())// 
			{
				//
				bool triggered = rt->acceptedTriggerThisFrame;//RuntimeCompをここで検証するか、RuntimeSystemの結果を流用するか検討必要
				inst.setBase(condTable.Triggered.v, triggered);

				rt->acceptedTriggerThisFrame = false;
			}

			// --- TimeElapsed bit ---
			if (condTable.TimeElapsed.valid() && rt->isActive && rt->hasPhaseDuration)
			{
				bool elapsed = (rt->phaseElapsed >= rt->curPhaseDuration);
				inst.setBase(condTable.TimeElapsed.v, elapsed);
			}
		}
	}

	void SkillFSMSystem::Update(const std::span<SkillPipelineEntry> ents, const float dt)
	{
		for (const auto& e : ents)
		{
			auto& inst = e.axis->inst;
			auto& reader = e.axis->reader;

			e.state->changedThisFrame = false;

			// ::Engine::Debug::Profiling::ScopedTimer timer("Skill");
			// FSM評価
			auto d = inst.tick(reader);


			if (inst.ApplyDecision(d))
			{
				const auto& fsm = *inst.fsm;
				const Engine::FSM::Core::StateID s_gid = fsm.local2GlobalState[inst.curState];
				const Engine::FSM::Core::ProfileID p_gid = fsm.local2GlobalProfile[inst.curProfile];

				e.state->Transition(e.state->curState, s_gid, e.state->curProf, p_gid);

				// Engine::FSM::Debug::PrintFSMInfo(e.e, inst, d);
			}
		}
	}

	void SkillLogicSystem::Update(const std::span<SkillPipelineEntry> ents, const float dt)
	{
		for (auto& e : ents)
		{

		}
	}

	void UpdateAllSkillFSM(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		SkillPipeline pipeline;
		BuildSkillPipeline(ctx, pipeline);


		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();

		SkillEnvSystem env{ ctx };
		SkillFSMSystem fsm{ ctx };
		SkillLogicSystem logic{ ctx };


		env.Update(pipeline, clock.fixedDt);
		fsm.Update(pipeline, clock.fixedDt);
	}
}