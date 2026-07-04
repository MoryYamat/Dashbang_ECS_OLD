#include "Game/00_Feature/Public/EffectFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/Audio/Public/GameAudioAPI.hpp"

namespace Game::Feature::Audio
{

	void AudioFeature::InitGameAudioSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Audio::InitAllGameAudioSystem(ctx);
	}
}