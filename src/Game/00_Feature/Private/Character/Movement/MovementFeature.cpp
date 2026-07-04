#include "MovementFeature.h"


#include "Game/Input/Public/InputApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/Character/Control/Public/ControlApi.hpp"


//
//
namespace Game::Feature::Character
{
	void MovementFeature::UpdateMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Input::Intent::MovementIntentMappingSystem::UpdatePlayerMovementIntent(ctx);
	}

	void MovementFeature::UpdateVelocity(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		
	}

	void MovementFeature::UpdateLogicFacing(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Character::Control::UpdateCharacterFacingIntent(ctx);
	}

	void MovementFeature::UpdateLogicPosition(Engine::WorldSystem::Core::WorldCtx& ctx) 
	{
		Game::Character::Control::UpdateActorPosition(ctx);
	}
}