#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Component/Private/Common/LifetimeComponent.hpp"
#include "Engine/Time/Private/WorldClock.hpp"

#include "Engine/Math/Private/MathUtils.h"
#include "Engine/Log/Public/LogApi.hpp"


namespace Game::Combat::HitBox
{
	using namespace Engine::Component;
	void HitBoxSpawnSystem::Update(const float dt)
	{
		auto& catalog = ctx.ww.GetResource<HitBoxCatalog>();
		auto& reqBuf = ctx.ww.GetResource<HitBoxSpawnRequestBuffer>();

		if (reqBuf.reqs.empty())
			return;

		for (const auto& req : reqBuf.reqs)
		{
			if (!req.hitbox.valid())continue;

			const auto idx = req.hitbox.v;
			if (idx >= catalog.hitBoxes.size()) continue;
			const auto& def = catalog.hitBoxes[idx];

			// Transform / Position
			// owner の 位置 + hd.offset から 初期位置決定
			const auto* ownerPos = ctx.rw.TryGet<Logic2DTransformComponent>(req.owner);
			if (!ownerPos) 
			{
				Engine::Log::Write(Engine::Log::Level::Info, "HitBoxSpawnSystem",
					"The OwnerEntity of the HitBox does not have a Logic2DComponent.");
				continue;
			}

			// === center: owner + ローカル offset(right/front) ===
			glm::vec2 worldOffset =
				def.offset.x * ownerPos->right +
				def.offset.y * ownerPos->front;
			glm::vec2 center = ownerPos->positionXZ + worldOffset;

			auto e = ctx.ww.Create();

			Logic2DTransformComponent lg{};
			lg.positionXZ = center;

			// 向き: オーナーのfront/rightにdef.angleを足したもの
			glm::vec2 baseFront = glm::normalize(ownerPos->front);
			glm::vec2 baseRight = glm::normalize(ownerPos->right);

			lg.front = Engine::Math::RotateVec2(baseFront, def.angle);
			lg.right = Engine::Math::RotateVec2(baseRight, def.angle);

			lg.rotation = ownerPos->rotation + def.angle;
			lg.scale = ownerPos->scale;
			ctx.ww.Add<Logic2DTransformComponent>(e, lg);

			LifetimeComponent lifetime{};
			lifetime.remainingLifetime = req.lifetime;
			ctx.ww.Add<LifetimeComponent>(e, lifetime);

			// HitBoxComponent
			HitBoxComponent hb{};
			hb.def = req.hitbox;
			hb.owner = req.owner;
			hb.skill = req.skill;
			ctx.ww.Add<HitBoxComponent>(e, hb);
		}

		reqBuf.clear();
	}

	void UpdateAllHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();

		HitBoxSpawnSystem hbSpawn{ ctx };
		hbSpawn.Update(clock.fixedDt);
	}
}