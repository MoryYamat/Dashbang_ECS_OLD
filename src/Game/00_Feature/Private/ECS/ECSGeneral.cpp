#include "ECSGeneral.hpp"

#include "Game/ECS/Public/GameECSAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

//
//
namespace Game::Feature::ECS
{
	void GameGeneralSytem::UpdateLifetimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::ECS::UpdateLifetimeSystem(ctx);
	}
}