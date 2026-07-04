#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Audio
{
	class AudioFeature
	{
	public:
		static void InitGameAudioSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
	private:

	};
}

namespace Game::Feature::VFX
{
	class VFXFeature
	{
	public:
		static void InitGameVFXSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
		static void Update(::Engine::WorldSystem::Core::WorldCtx& ctx);
	private:

	};
}