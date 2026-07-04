#include "Game/Character/Animation/Private/InternalApi.hpp"
#include "Game/Character/Animation/Public/AnimationApi.hpp"

#include "Engine/Time/Private/WorldClock.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Graphics/AnimatorComponent.hpp"
#include "Engine/Component/Private/Graphics/MeshComponent.hpp"

#include "Game/Character/Animation/Public/LocomAnimComponent.hpp"
#include "Game/Character/Animation/Public/SkillAnimComponent.hpp"

#include <string>
#include <iostream>


namespace
{

}

namespace Game::Character::Animation
{
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::Component;

	void ApplyFinalAnimationDecisionSystem::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = ViewWhere(ctx.rw, All<AnimatorComponent, MeshComponent, FinalAnimDecisionComponent>{});

		for (const auto& e : ents)
		{
			auto& anim = ctx.ww.Get<AnimatorComponent>(e);
			const auto& mesh = ctx.ww.Get<MeshComponent>(e);
			const auto& dec = ctx.ww.Get<FinalAnimDecisionComponent>(e);

			if (!dec.valid) continue;

			bool same = false;
			if (anim.clipIndex >= 0 && anim.clipIndex < (int)mesh.modelData.clips.size())
			{
				same = (mesh.modelData.clips[(size_t)anim.clipIndex].name == dec.clipKey);
			}
			if (!same)
			{
				if (!SetClipByNameRange(mesh.modelData, anim, dec.clipKey, dec.loop, dec.starTime, dec.endTime, dec.loopWithinRange))
				{
					// フォールバック
					SetClipByName(mesh.modelData, anim, "idle_default", true);

					Engine::Log::Write(Engine::Log::Level::Error, 
						"[ApplyFinalAnimDecisionSystem]", "No matching clip keys were found.");
				}
				else
				{
					anim.startTime = dec.starTime;
					anim.endTime = dec.endTime;
					anim.loopWithinRange = dec.loopWithinRange;

				}
			}

			anim.speed = dec.playRate;
		}
	}

	void AnimationArbiterSystem::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = ViewWhere(ctx.rw, AND(All<FinalAnimDecisionComponent>{},
			Any<Movement::MovementAnimDecisionComponent, Skill::SkillAnimRequestComponent>{}));

		for (const auto& e : ents)
		{
			auto& out = ctx.ww.Get<FinalAnimDecisionComponent>(e);

			out.valid = false;
			out.clipKey.clear();
			out.loop = true;
			out.playRate = 1.0f;

			int bestPrio = -99999;

			auto consider = [&](bool v, int prio, const std::string& key, bool loop, float rate
				,float startT, float endT, float loopRange)
				{
					if (!v)return;
					if (!out.valid || prio > bestPrio)
					{
						out.valid = true;
						out.clipKey = key;
						out.loop = loop;
						out.playRate = rate;
						out.starTime = startT;
						out.endTime = endT;
						out.loopWithinRange = loopRange;
						bestPrio = prio;
					}
				};

			// TODO: この構造は問題があるので変更が必要
			if (ctx.rw.Has<Movement::MovementAnimDecisionComponent>(e))
			{
				auto& mv = ctx.ww.Get<Movement::MovementAnimDecisionComponent>(e);
				consider(mv.valid, 10, mv.clipKey, mv.loop, mv.playRate, 0.0f, -1.0f, true);
			}
			if (ctx.rw.Has<Skill::SkillAnimRequestComponent>(e))
			{
				// TODO: AnimID→clipIndex 解決を O(1)で行うように ここでは、IDのみを持つように変更する
				// 
				auto& sk = ctx.ww.Get<Skill::SkillAnimRequestComponent>(e);
				consider(sk.active, sk.priority, sk.clipKey, sk.loop, sk.playRate, sk.startTime, sk.endTime, sk.loopWithinRange);
			}

		}
	}

		//void AnimationArbiterSystem::Update(Engine::ECS::EntityMgr& ecs)
	//{
	//	for (auto e : ecs.view<
	//		FinalAnimationDecisionComponent,
	//		MvAnim::MovementAnimDecisionComponent,
	//		SkAnim::SkillAnimDecisionComponent,
	//		CCAnim::CCAnimDecisionComponent
	//	>())
	//	{
	//		auto& out = Ops::Get<FinalAnimationDecisionComponent>(ecs, e);
	//		const auto& mv = Ops::Get<MvAnim::MovementAnimDecisionComponent>(ecs, e);
	//		const auto& sk = Ops::Get<SkAnim::SkillAnimDecisionComponent>(ecs, e);
	//		const auto& cc = Ops::Get<CCAnim::CCAnimDecisionComponent>(ecs, e);
	
	//		// TODO: 条件のハードコードをやめ、優先度による解決、データ駆動で解決できるようにする必要がある
	//		// FIXME:条件のハードコードをやめ、優先度による解決、データ駆動で解決できるようにする必要がある
	//		if (mv.valid)
	//		{
	//			out.valid = true;
	//			out.clipKey = mv.clipKey;
	//			out.loop = mv.loop;
	//			out.playRate = mv.playRate;
	//		}
	//		else
	//		{
	//			out.valid = false;
	//		}
	//		if (sk.valid)
	//		{
	//			out.valid = true;
	//			out.clipKey = sk.clipKey;
	//			out.loop = sk.loop;
	//			out.playRate = sk.playRate;
	//		}
	//		//else
	//		//{
	//		//	out.valid = false;
	//		//}
	//		if (cc.valid)
	//		{
	//			out.valid = true;
	//			out.clipKey = cc.clipKey;
	//			out.loop = cc.loop;
	//			out.playRate = cc.playRate;
	//			// std::cout << "here\n";
	//		}
	
	//		// 将来的に CC / Skill の優先度ロジックを追記したりする
	//	}
	//}

	void UpdateAllLocomResolverSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Movement::LocomQuerySystem(ctx);
		Movement::MovementAnimationResolver::Update(ctx);
	}

	void UpdateAllSkillAnimResolverSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& clock = ctx.rw.GetResource<::Engine::Time::WorldClockData>();
		Skill::SkillAnimRequestLifetimeSystem sls{ ctx };
		sls.Update(clock.fixedDt);
	}

	void UpdateAllCharacterAnimSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		UpdateAllLocomResolverSystem(ctx);
		UpdateAllSkillAnimResolverSystem(ctx);
		AnimationArbiterSystem::Update(ctx);
		ApplyFinalAnimationDecisionSystem::Update(ctx);
	}
}