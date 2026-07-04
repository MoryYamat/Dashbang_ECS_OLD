#include "CollisionFeature.h"

#include "Game/Collision/Public/CollisionApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Feature
{

	// Collision バッファの初期化
	void CollisionFeature::InitCollisionBuffer(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Collision::InitAllCollisionSystem(ctx);
	}

	void CollisionFeature::UpdateCollisionSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Collision::UpdateCollisionSystem(ctx);
	}

	void CollisionFeature::SyncLogicCollision(Engine::WorldSystem::Core::WorldCtx& ctx) 
	{
		
	}
}