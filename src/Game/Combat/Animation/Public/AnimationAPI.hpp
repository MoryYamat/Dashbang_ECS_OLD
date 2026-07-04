#pragma once
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Combat/Animation/Public/AnimationFwd.hpp"

namespace Game::Combat::Animation
{
	// internal
	void RegisterAnimationDTO(AnimBuilder& builder);


	// public
	void InitAllAnimSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);

	void UpdateAllAnimSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}