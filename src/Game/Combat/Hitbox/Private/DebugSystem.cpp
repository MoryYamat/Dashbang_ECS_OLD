#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"

#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Physics/Public/Geometry2DTypes.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Debug/Public/DebugDrawAPI.hpp"

#include "Engine/Math/Private/MathUtils.h"

#include <iostream>

namespace
{
	using namespace Game::Combat::HitBox;

	struct HitBoxDebugDrawSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx_;

		void Update();
	};

	void HitBoxDebugDrawSystem::Update()
	{
		auto& dbgBuf = ctx_.ww.GetResource<Engine::Debug::CommandBuffer>();
		auto& catalog = ctx_.rw.GetResource<HitBoxCatalog>();

		using namespace Engine::WorldSystem::Query;
		auto ents = ViewWhere(ctx_.rw, All<HitBoxComponent, Engine::Component::Logic2DTransformComponent>{});

		for (const auto& e : ents)
		{
			const auto& hb = ctx_.ww.Get<HitBoxComponent>(e);
			const auto& tr = ctx_.ww.Get<Engine::Component::Logic2DTransformComponent>(e);

			if (!hb.def.valid()) continue;
			auto idx = hb.def.v;
			if (idx >= catalog.hitBoxes.size()) continue;

			const auto& def = catalog.hitBoxes[idx];

			glm::vec2 center = tr.positionXZ;

			//
			if (def.shape == Engine::Physics::ShapeKind::Circle2D)
			{
				Engine::Physics::Circle2D c{};
				c.center = center;
				c.radius = def.radius;
				Engine::Debug::AddCircle(dbgBuf, c, { 1,0,0,1 });
			}
			else if (def.shape == Engine::Physics::ShapeKind::Box2D)
			{
				Engine::Physics::Box2D b{};
				b.center = center;
				b.halfExtents = { def.halfExtents };
				Engine::Debug::AddBox(dbgBuf, b, { 1,0,0,1 });
			}
			else if (def.shape == Engine::Physics::ShapeKind::Obb2D)
			{
				Engine::Physics::Obb2D o{};
				o.center = center;
				o.halfExtents = def.halfExtents;

				// オーナー
				glm::vec2 baseX = tr.right;		// ローカルX軸
				glm::vec2 baseY = tr.front;		// ローカルY軸

				o.axisX = Engine::Math::RotateVec2(baseX, def.angle);
				o.axisZ = Engine::Math::RotateVec2(baseY, def.angle);

				Engine::Debug::AddObb(dbgBuf, o, { 1,0,0,1 });
			}
		}
	}
}

namespace Game::Combat::HitBox
{
	void UpdateAllHitBoxDebugSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& cfg = ctx.ww.GetResource<Engine::Debug::Config>();
		if (!cfg.enabled)
			return;

		HitBoxDebugDrawSystem hbdebug{ ctx };
		hbdebug.Update();
	}
}