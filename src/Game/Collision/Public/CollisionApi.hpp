#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Collision/Public/CollisionFwd.hpp"
#include "Game/ECS/Public/GameECSFwd.hpp"

#include "Game/Collision/Public/CollisionMaskComponent.hpp"

namespace Game::Collision
{
	void CollisionDetectionSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	void InitAllCollisionSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	void UpdateCollisionSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	// TODO: 以下ローカルのstatic 関数化してAPI公開不要かどうか検討
	bool shouldCollideWithCat(
		CollisionMaskComponent& A,
		CollisionMaskComponent& B
	);

	bool shouldCollideWithRel(
		const CollisionMaskComponent& A,
		const CollisionMaskComponent& B,
		Relation rab,
		Relation rba
	);

	Game::ECS::Component::Team resolveTeam(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::ECS::Core::Entity e);

	// 相対関係計算関数
	Relation computeRelation(Engine::WorldSystem::Core::WorldCtx& ctx,
		Engine::ECS::Core::Entity a, Engine::ECS::Core::Entity b);
}