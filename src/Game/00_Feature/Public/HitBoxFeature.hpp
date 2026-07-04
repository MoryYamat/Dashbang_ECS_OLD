#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Combat
{
	class AnimFeature
	{
	public:
		static void InitAnimSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateAnimSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	class HitBoxFeature
	{
	public:
		static void InitHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	private:
	};
}