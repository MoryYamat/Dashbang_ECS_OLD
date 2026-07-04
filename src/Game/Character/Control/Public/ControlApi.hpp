#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Combat/Skill/Public/CoreFwd.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace Game::Character::Control
{
	// Intent
	void UpdateCharacterMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdateCharacterFacingIntent(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdatePlayerSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx);


	// Logic
	void UpdateActorPosition(Engine::WorldSystem::Core::WorldCtx& ctx);
}