#pragma once

#include <string_view>
#include <filesystem>
namespace Engine::Audio
{
	class IAudioBackend
	{
	public:
		virtual ~IAudioBackend() = default;

		// 音声デバイス / エンジン 初期化
		virtual bool initialize() = 0;

		// 終了処理 (initialize成功後に呼ばれる想定)
		virtual void shutdown() noexcept = 0;

		// ワンショット再生 (path は実行ファイル基準など、上位が解決済み想定)
		virtual void play_one_shot(std::string_view path, float volume = 1.0f) = 0;
		virtual void play_one_shot(const std::filesystem::path& path, float volume = 1.0f) = 0;

	private:

	};

}