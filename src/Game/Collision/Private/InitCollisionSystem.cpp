#include "Game/Collision/Public/CollisionApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Collision/Public/Types.hpp"

namespace Game::Collision
{
	static void InitCollisionResultStorage(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& strg = ctx.ww.CreateResource<CollisionResultStorage>();
	}


	void InitAllCollisionSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		InitCollisionResultStorage(ctx);
	}
}