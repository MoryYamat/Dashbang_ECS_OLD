#include "Game/Character/FSM/Public/MovementTypes.hpp"
#include "Game/Character/FSM/Public/MovementAxisApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"
#include "Engine/FSM/Public/Core/StateEvents.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"
#include "Game/Character/Stats/Public/StatsComponent.hpp"
#include "Game/Character/Control/Public/IntentComponent.hpp"

#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Private/AllFSMSystem.hpp"

#include "Engine/Debug/Public/ProfilingAPI.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace
{
	using namespace Engine::Component;
	using namespace Engine::FSM::Core;
	using namespace Engine::WorldSystem::Query;
	using namespace Game::Character::FSM::Movement;

	// Entityのフィルタリング
	void BuildMovementPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, MovementPipeline& out)
	{
		out.clear();
		auto ents = ViewWhere(ctx.rw, All<
			MovementAxisComp, 
			MovementStateComp, 
			MovementModifierComponent,
			Velocity2DComponent>{});


		for (const auto& e : ents)
		{
			auto* axis = ctx.ww.TryGet<MovementAxisComp>(e);
			auto* state = ctx.ww.TryGet<MovementStateComp>(e);
			auto* vel = ctx.ww.TryGet<Engine::Component::Velocity2DComponent>(e);

			out.push_back(MovementPipelineEntry{
					e,
					axis,
					state,
					vel
				});
		}
	}

	void ExecuteMovementOps(std::uint32_t mask, Engine::WorldSystem::Core::WorldCtx& ctx,
		const MovementPipelineEntry& ent, float dt)
	{
		// 
		if (mask & OpBit(MovementOpKind::ApplyVelocityFromIntent))
		{
			// intent -> velocity
			SetMovementVelComp(ent, ctx);
		}
		if (mask & OpBit(MovementOpKind::ZeroVelocity))
		{
			// TODO: velocity -> 0
			SetZeroVel(ent, ctx);
		}

		// 将来その他のOpKind::についての処理を追加
	}

	void UpdateMovementModifiers(Engine::WorldSystem::Core::WorldCtx& ctx, float dt)
	{

		auto ents = ViewWhere(ctx.rw, All<MovementModifierComponent>{});

		for (const auto& e : ents)
		{
			auto& comp = ctx.ww.Get<MovementModifierComponent>(e);

			for (auto& m : comp.entries)
			{
				m.remaining -= dt;
			}

			std::erase_if(comp.entries, [](const MovementModifierEntry& m)
				{
					return m.remaining <= 0.0f;
				});
		}
	}
}

namespace Game::Character::FSM::Movement
{
	using namespace Engine::Component;
	using namespace Engine::FSM::Core;
	using namespace Engine::WorldSystem::Query;

	void MovementEnvSystem::Update(std::span<MovementPipelineEntry> ents, const float dt)
	{
		// std::cout << "here\n";
		for (auto& entry : ents)
		{
			// std::cout << "here\n";
			auto& inst = entry.axis->inst;
			auto& reader = entry.axis->reader;

			inst.beginFrame(dt);

			auto* moveIntent = ctx.ww.TryGetWithWarnOnce<Game::Character::Control::MovingIntentComponent>(entry.e);
			if (!moveIntent) continue;
			reader.movementInputMag = moveIntent ? glm::length2(moveIntent->direction) : 0.0f;

			// reader.movementInputMag = moveIntentDir ? glm::length2(testDir) : 0.0f;
		}
	}

	void MovementFSMSystem::Update(std::span<MovementPipelineEntry> ents, const float dt)
	{
		for (auto& entry : ents)
		{
			auto& inst = entry.axis->inst;
			auto& reader = entry.axis->reader;

			entry.state->changedThisFrame = false;

			// ::Engine::Debug::Profiling::ScopedTimer timer("Movement");
			// FSM評価
			Engine::FSM::Core::Decision d = inst.tick(reader);

			if (inst.ApplyDecision(d))
			{
				const auto& fsm = *inst.fsm;
				const Engine::FSM::Core::StateID s_gid = fsm.local2GlobalState[inst.curState];
				const Engine::FSM::Core::ProfileID p_gid = fsm.local2GlobalProfile[inst.curProfile];
				//entry.state->prevState = entry.state->curState;
				//entry.state->curState = gid;
				//entry.state->changedThisFrame = d.changed;

				entry.state->Transition(entry.state->curState, s_gid, entry.state->curProf, p_gid);

				// Engine::FSM::Debug::PrintFSMInfo(entry.e, inst, d);
			}

		}
	}




	void MovementLogicSystem::Update(std::span<MovementPipelineEntry> ents, const float dt)
	{
		auto* ltbl = ctx.rw.TryGetResource<MovementLogicTable>();

		if (!ltbl) return;

		for (auto& entry : ents)
		{
			auto* st = entry.state;
			if (!st) continue;

			StateEventView sev(
				st->prevState, 
				st->curState, 
				st->prevProf,
				st->curProf,
				st->changedThisFrame
			);

			for (const auto& rule : ltbl->entries)
			{
				// Trigger種別に応じてフィルタ
				const auto& k = rule.key;
				bool fire = sev.match(k.state, k.profile, k.trigger);
				if (!fire) continue;

				ExecuteMovementOps(rule.opMask, ctx, entry, dt);
			}


			entry.state->ResetChanged();
		}
	}

	void UpdateMovementAll(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		MovementPipeline pipeline;
		BuildMovementPipeline(ctx, pipeline);

		// ワールドリソースからとる予定
		const auto& dt = ctx.rw.GetResource<Engine::Time::WorldClockData>();


		UpdateMovementModifiers(ctx, dt.fixedDt);

		MovementEnvSystem envSys{ ctx };
		MovementFSMSystem fsmSys{ ctx };
		MovementLogicSystem logicSys{ ctx };


		envSys.Update(pipeline, dt.fixedDt);
		fsmSys.Update(pipeline, dt.fixedDt);
		logicSys.Update(pipeline, dt.fixedDt);
	}
}