#include "Game/Collision/Public/CollisionApi.hpp"
#include "Game/Collision/Public/CollisionMaskComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/ECS/Private/Component/TeamComponent.h"



namespace Game::Collision
{
	bool shouldCollideWithCat(
		CollisionMaskComponent& A,
		CollisionMaskComponent& B
	)
	{
		const bool catOK =
			((A.collideCategoryMask & static_cast<uint32_t>(B.category)) != 0) &&
			((B.collideCategoryMask & static_cast<uint32_t>(A.category)) != 0);
		if (!catOK) return false;

		return true;
	}

	bool shouldCollideWithRel(
		const CollisionMaskComponent& A,
		const CollisionMaskComponent& B,
		Relation rab,
		Relation rba
	)
	{

		const bool relOK = has(A.relationMask, rab) && has(B.relationMask, rba);
		if (!relOK) return false;

		return true;
	};

	Game::ECS::Component::Team resolveTeam(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::ECS::Core::Entity e)
	{
		if (ctx.ww.Has<Game::ECS::Component::TeamComponent>(e))// エージェントなどの場合
		{
			return ctx.ww.Get<Game::ECS::Component::TeamComponent>(e).team;
		}
		//if (ctx.ww.Has<>(e))// スキルComponentの場合
		//{

		//}

		return Game::ECS::Component::Team::Neutral;

		//if (ecs.hasComponent<TeamComponent>(e))
		//{
		//	return ecs.get<TeamComponent>(e).team;
		//}
		//if (ecs.hasComponent<SkillOwnerComponent>(e))
		//{
		//	auto owner = ecs.get<SkillOwnerComponent>(e).caster;
		//	return resolveTeam(ecs, owner);
		//}

	};

	// 相対関係計算関数
	Relation computeRelation(Engine::WorldSystem::Core::WorldCtx& ctx,
		Engine::ECS::Core::Entity a, Engine::ECS::Core::Entity b)
	{
		using namespace Game::ECS::Component;

		if (a.id == b.id)
			return Relation::Self;

		// TODO: 以下のFIXMEは問題にならないかもしれない．キチンとシナリオを追って完全性を証明する必要あり
		// FIXME: 現在，caster == a : Ally になってしまう．
		// これは，caster == a : ならば，Selfにならなければいけない．
		// つまり， ally: if(a is not skillEntity or b is not skillEntity) and if(ta == tb)
		Team ta = resolveTeam(ctx, a);
		Team tb = resolveTeam(ctx, b);

		if (ta == Game::ECS::Component::Team::Neutral || tb == Game::ECS::Component::Team::Neutral)
			return Relation::Neutral;

		if (ta == tb)
			return Relation::Ally;


		return Relation::Enemy;
	};


}