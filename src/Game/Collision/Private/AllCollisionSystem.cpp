#include "Game/Collision/Public/CollisionApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


namespace Game::Collision
{
	void UpdateCollisionSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		CollisionDetectionSystem(ctx);
	}
}