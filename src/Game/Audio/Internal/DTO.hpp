#pragma once
#include "Game/Audio/Generated/SoundKeys.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"
#include <cstdint>
#include <string>

namespace Game::Audio
{
	struct AudioDTO
	{
		SoundKey key;
		std::string name;
		::Engine::Audio::SoundDef def;
	};
}