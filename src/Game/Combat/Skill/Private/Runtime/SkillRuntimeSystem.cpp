#include "Game/Combat/Skill/Public/RuntimeAPI.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"

#include "Game/Character/Control/Public/IntentComponent.hpp"
#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"

#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Log/Public/LogApi.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"

#include "Game/Combat/Skill/Public/CoreAPI.hpp"
#include "Game/Combat/Skill/Internal/Runtime/RuntimeTypes.hpp"

#include <span>
#include <vector>

namespace
{
	using namespace Engine::WorldSystem::Query;
	using namespace Game::Combat::Skill;
	using namespace Game::Combat::Skill::Runtime;

	// 処理対象entity の一括抽出
	void BuildSkillRuntimePipeline(Engine::WorldSystem::Core::WorldCtx& ctx, SkillRuntimePipeline& out)
	{
		out.clear();
		auto ents = ViewWhere(ctx.rw, All<SkillRuntimeComp>{});

		for (const auto& e : ents)
		{
			auto* state = ctx.rw.TryGet<Game::Character::FSM::Skill::SkillStateComp>(e);
			auto* runtime = ctx.ww.TryGet<SkillRuntimeComp>(e);
			auto* intent = ctx.rw.TryGet<Game::Character::Control::SkillIntentComponent>(e);
			auto* logic2d = ctx.rw.TryGet<Engine::Component::Logic2DTransformComponent>(e);

			out.push_back(SkillRuntimePipelineEntry{
				 .e = e,
				 .state = state,
				 .runtimeComp = runtime,
				 .intent = intent,
				 .logic2d = logic2d
				});
		}
	}

	bool CanTrigger(SkillRuntimeComp& runtime, const SkillRequest& req)
	{
		// TODO: FSM状態/クールダウン/ほかの制約などから判定

		return true;
	}

	void EnqueueTrigger(SkillRuntimeComp& runtime, const SkillRequest& req)
	{
		runtime.acceptedTriggerThisFrame = true;
		runtime.pendingTriggers.push_back(req);
		// std::cout << "here\n";
	}

	float FindPhaseDuration(const CanonicalSkillData& data, Engine::FSM::Core::StateID s)
	{
		for (const auto& ph : data.phases)
		{
			if (ph.state.v == s.v)
			{
				return ph.duration;
			}
		}

		return 0.f;
	}
}


namespace Game::Combat::Skill::Runtime
{
	using namespace Engine::WorldSystem::Query;
	using namespace Game::Combat::Skill::Binding;


	// 現在のSkillRuntimeCompを更新する(トリガー/ 経過時間(ElapsedTime)/ など
	// SkillRuntimeComp -> SkillFSM(Env)
	void SkillTriggerSystem::Update(const std::span<SkillRuntimePipelineEntry> ents, const float dt)
	{
		for (const auto& e : ents)
		{
			//std::cout << "here\n";

			for (const auto& req : e.intent->requests)
			{
				// TODO: コマンドの処理(同時押し/ボタン組合せetc.)
				if (!CanTrigger(*e.runtimeComp, req))
					continue;

				EnqueueTrigger(*e.runtimeComp, req); 
			}

			//e.intent->requests.clear();// Control側で実施
		}
	}

	// TODO: レイヤーをへらしてもよいかもしれない
	void SkillRuntimeSystem::Update(const std::span<SkillRuntimePipelineEntry> ents, const float dt)
	{
		auto& catalog = ctx_.rw.GetResource<SkillCatalog>();

		for (const auto& e : ents)
		{
			auto& rt = e.runtimeComp;

			// FSM 側 の state をコピー
			rt->state = e.state->curState;

			SkillRequest chosen{};
			bool hasTrigger = false;

			// pendingTriggers からトリガーを取り出す
			// ver.1 では 「一つだけ採用、残りは無視」		// 将来的にコマンド合成も
			if (!rt->pendingTriggers.empty())
			{
				chosen = rt->pendingTriggers.front();
				hasTrigger = true;
			}

			// スキルが非アクティブで、トリガーが来ていたら開始
			if (!rt->isActive && hasTrigger)
			{
				// skillID から SkillCatalog を引く (id.v を index にする簡単な方法でOK)
				auto idx = chosen.skill.v;
				if (idx >= catalog.skills.size())
				{
					// 無効なSkillID

					Engine::Log::Write(Engine::Log::Level::Trace, "SkillRuntimeSystem", "Skill ID resolution error");
				}
				else
				{
					const auto& skillData = catalog.skills[idx];

					rt->curSkill = chosen.skill;
					rt->curSkillData = &skillData;
					rt->isActive = true;

					rt->ResetTimeElapsed();				// total/phase/prev = 0


					// 開始時点のステートに対応するduration を決定
					rt->curPhaseDuration = FindPhaseDuration(skillData, rt->state);
					rt->hasPhaseDuration = (rt->curPhaseDuration > 0.f);


					// std::cout << "here\n";
				}
			}

			// changedThisFrame → SkillFSM側システムで管理
			const bool stateChanged = e.state->changedThisFrame;

			// スキルActive中
			if (rt->isActive && rt->curSkillData)
			{
				if (stateChanged)
				{
					rt->phaseElapsed = 0.f;
					rt->curPhaseDuration =
						FindPhaseDuration(*rt->curSkillData, rt->state);
					rt->hasPhaseDuration = (rt->curPhaseDuration > 0.f);
				}

				// 直前のフェーズ時間を保存してからdtを足す
				rt->prevPhaseElapsed = rt->phaseElapsed;
				rt->totalElapsed += dt;
				rt->phaseElapsed += dt;

				// - FSM Env を組みたてて評価
				// - 状態遷移に応じて phaseElapsed をリセット
				// 

				// std::cout << rt->phaseElapsed << "\n";
			}

			rt->ResetInput();
		}
	}

	// runtime → コマンド発行
	void SkillLogicCommandSystem::Update(const std::span<SkillRuntimePipelineEntry> ents, const float dt)
	{
		const auto& catalog = ctx_.rw.GetResource<SkillCatalog>();
		auto& cmdBuf = ctx_.ww.GetResource<SkillLogicCommandBuffer>();
		cmdBuf.clear();

		for (const auto& e : ents)
		{
			auto& rt = e.runtimeComp;
			if (!rt->isActive || !rt->curSkillData)
				continue;

			const auto& skill = rt->curSkillData;

			const auto curState = rt->state;
			const float prevT = rt->prevPhaseElapsed;
			const float curT = rt->phaseElapsed;

			for (const auto& eff : skill->effects)
			{
				if (eff.state.v != curState.v)
					continue;

				// [prevT, curT] で timeOffset を跨いだら発火
				if (prevT <= eff.timeOffset && eff.timeOffset < curT)		// 検証必要
				{
					SkillLogicCommand cmd{};
					cmd.owner = e.e;
					cmd.skill = skill->id;
					cmd.logic2d = e.logic2d;
					cmd.state = eff.state;
					cmd.effectTime = eff.timeOffset;
					cmd.lifetime = eff.lifetime;
					cmd.value = eff.value;
					

					switch (eff.kind)
					{
					case EffectKind::SpawnHitBox:
					{
						cmd.kind = LogicCommandKind::SpawnHitBox;
						break;
					}
					case EffectKind::ModifyMoveSpeed:
					{
						cmd.kind = LogicCommandKind::ModifyMoveSpeed;
						break;
					}
					case EffectKind::PlayAnim:
					{
						cmd.kind = LogicCommandKind::PlayAnim;
						break;
					}
					case EffectKind::PlaySFX:
					{
						cmd.kind = LogicCommandKind::PlaySFX;
						break;
					}
					case EffectKind::PlayVFX:
					{
						cmd.kind = LogicCommandKind::PlayVFX;
						break;
					}
					}

					cmdBuf.cmds.push_back(cmd);
				}
			}
		}
	}


	// すべてのSkillRuntimeSystemを更新
	void UpdateAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// struct // intent->skilltrigger
		// struct // update runtimecomp
		// struct // update effect commands
		SkillRuntimePipeline pipeline;
		BuildSkillRuntimePipeline(ctx, pipeline);
		
		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();

		SkillTriggerSystem trSys{ ctx };
		SkillRuntimeSystem rtSys{ ctx };
		SkillLogicCommandSystem LCSys{ ctx };
		SkillCommandExecSystem execSys{ ctx };

		trSys.Update(pipeline, clock.fixedDt);
		rtSys.Update(pipeline, clock.fixedDt);
		LCSys.Update(pipeline, clock.fixedDt);
		execSys.Update(clock.fixedDt);
	}
}