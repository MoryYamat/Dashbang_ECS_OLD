#pragma once

#include "Engine/Audio/Public/AudioFwd.hpp"
#include "Game/Audio/Public/GameAudioFwd.hpp"

#include "Game/Audio/Internal/DTO.hpp"


#include <vector>
#include <string>

namespace Game::Audio
{
	struct BuildErrors
	{
		std::vector<std::string> msgs;
		bool ok() const noexcept { return msgs.empty(); }
		void error(std::string msg) { msgs.emplace_back(msg); }
	};


	class AudioCatalogBuilder
	{
	private:
		std::vector<AudioDTO> defs_;
		BuildErrors errs_;

	public:

		void Add(AudioDTO dto) { defs_.push_back(std::move(dto)); }

		void Build(::Engine::Audio::AudioCatalogResource& resource, AudioIds& ids);

		bool Ok() { return errs_.ok(); }
	};
}