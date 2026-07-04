#include "Game/Combat/Animation/Internal/AnimTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/Log/Public/LogApi.hpp"

#include "Game/Character/Animation/Public/SkillAnimComponent.hpp"

#include <iostream>

namespace Game::Combat::Animation
{
	void PlayAnimSystem::Update(const float dt)
	{
		auto& catalog = ctx.ww.GetResource<AnimCatalog>();
		auto& reqBuf = ctx.ww.GetResource<PlayAnimRequestBuffer>();

		if (reqBuf.reqs.empty())
			return;

		for (const auto& req : reqBuf.reqs)
		{
			if (!req.anim.valid())
				continue;

			const auto idx = req.anim.v;
			if (idx >= catalog.anims.size()) continue;
			const auto& def = catalog.anims[idx];

			
			// やるべきこと
			// (AnimatorComponent 取得) → Animation Request 作成のほうが良いかもしれない．
			// clip名を解決し、idを取得
			// 制御情報(loop / start-end frame / speed ) 代入
			// Animation Request 追加
			// 再生側で解決
			//

			// auto* dec = ctx.ww.TryGet<Game::Character::Animation::Skill::SkillAnimDecisionComponent>(req.owner);
			auto* const reqComp = ctx.ww.TryGet<::Game::Character::Animation::Skill::SkillAnimRequestComponent>(req.owner);
			if (!reqComp)
			{
				Engine::Log::Write(Engine::Log::Level::Error, "AnimPlaySystem",
					"The OwnerEntity does not have a SkillAnimQuewryComponent.");
				continue;
			}

			reqComp->active = true;
			reqComp->id = req.anim;
			reqComp->clipKey = def.clip;
			reqComp->loop = req.loop;
			reqComp->playRate = req.speed;
			reqComp->startTime = req.start_frame;
			reqComp->endTime = req.end_frame;
			reqComp->loopWithinRange = req.loop;
			reqComp->ttl = 1.0f;				// 仮 後でclip durationに
			reqComp->priority = 100;

			// dec->clipKey = def.clip;
			// dec->loop = req.loop;
			// dec->playRate = req.speed;
			// dec->valid = true;


			// std::cout << "clipKey: " << dec->clipKey << "\n";
		}


		reqBuf.clear();
	}

	void UpdateAllAnimSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();

		PlayAnimSystem sys{ ctx };
		sys.Update(clock.fixedDt);// clock どっち
	}
}