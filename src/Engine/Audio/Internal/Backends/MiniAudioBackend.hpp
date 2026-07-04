#pragma once

#include "Engine/Audio/Public/IAudioBackend.hpp"

#include <string_view>
#include <memory>
#include <filesystem>

namespace Engine::Audio
{
	class MiniAudioBackend final : public IAudioBackend
	{
	public:

		MiniAudioBackend();
		~MiniAudioBackend() override;

		MiniAudioBackend(const MiniAudioBackend&) = delete;
		MiniAudioBackend& operator=(const MiniAudioBackend&) = delete;

		MiniAudioBackend(MiniAudioBackend&&) noexcept;
		MiniAudioBackend& operator=(MiniAudioBackend&&) noexcept;

		bool initialize() override;
		void shutdown() noexcept override;

		void set_max_voices(std::size_t n);

		void play_one_shot(std::string_view path, float volume = 1.0f) override;
		void play_one_shot(const std::filesystem::path& path, float volume = 1.0f)override;
		void pump();	// ñàÉtÉåÅ[ÉÄâÒé˚
	private:

		struct Impl;
		std::unique_ptr<Impl> impl_;
	};
}