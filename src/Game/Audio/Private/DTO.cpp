#include "Game/Audio/Internal/DTO.hpp"
#include "Game/Audio/Internal/GameAudioInternalAPI.hpp"

#include "Engine/Audio/Public/AudioAPI.hpp"
#include "Engine/Audio/Public/AudioFwd.hpp"
#include "Game/Audio/Private/CatalogBuilder.hpp"

#include "Game/Audio/Generated/SoundKeys.hpp"

namespace
{
	using namespace ::Engine::Audio;
	using namespace ::Game::Audio;

	void make_AudioResources(AudioCatalogBuilder& builder)
	{
		AudioDTO sfx_text{ .key = SoundKey::sfx_test
							, .name = "sfx_text"
							, .def = SoundDef{
								.path = "res://Sounds/musicholder-sword-sound-260274.wav"		
								, .bus = AudioBus::SFX
								, .defaultVolume = 1.0f
								, .stream = false} 
						};


		builder.Add(sfx_text);
	}
}

namespace Game::Audio
{
	void RegisterAudioDTO(AudioCatalogBuilder& builder)
	{
		make_AudioResources(builder);
	}
}