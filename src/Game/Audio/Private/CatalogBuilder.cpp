#include "Game/Audio/Private/CatalogBuilder.hpp"

#include "Engine/Audio/Public/AudioAPI.hpp"

#include "Game/Audio/Public/GameAudioAPI.hpp"

namespace Game::Audio
{
	using namespace Engine::Audio;

	void AudioCatalogBuilder::Build(::Engine::Audio::AudioCatalogResource& resource, AudioIds& ids)
	{
		auto& catalog = resource.catalog;

		for (const auto& def : defs_)
		{
			const SoundID id = catalog.register_sound(def.name, def.def);
			if (!id.is_valid())
			{
				errs_.error("Sound registration failed (invalid SoundID)");
				continue;
			}
			ids.set(def.key, id);
		}
	}
}