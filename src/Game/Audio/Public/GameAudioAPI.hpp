#pragma once

#include "Engine/Audio/Public/AudioAPI.hpp"
#include "Engine/Audio/Public/AudioFwd.hpp"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Audio/Generated/SoundKeys.hpp"
#include "Game/Audio/Public/GameAudioFwd.hpp"

#include <array>
#include <cstddef>

namespace Game::Audio
{
	// public
	struct AudioIds
	{
		std::array<::Engine::Audio::SoundID, static_cast<std::size_t>(SoundKey::COUNT)> ids{};

		AudioIds()
		{
			ids.fill(::Engine::Audio::SoundID::Invalid());
		}

		::Engine::Audio::SoundID get(SoundKey key) const noexcept
		{
			return ids[static_cast<std::size_t>(key)];
		}

		void set(SoundKey key, ::Engine::Audio::SoundID id) noexcept
		{
			ids[static_cast<std::size_t>(key)] = id;
		}
	};


	// Public
	bool InitAllGameAudioSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
}