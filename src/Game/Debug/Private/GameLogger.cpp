#include "Game/Debug/Public/GameLogger.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <string>
#include <string_view>
#include <cstdio>

namespace Game::Debug
{
	void GameLogger::Write(Engine::Log::Level level,
						std::string_view channel,
						std::string_view message)
	{
		// コンソールや VS Outputなどへ出す
		std::fprintf(stderr,
			"[Game][%.*s][%u] %.*s\n",
			(int)channel.size(), channel.data(),
			static_cast<unsigned>(level),
			(int)message.size(), message.data());

		// メモリに保存
		entries_.push_back(LogEntry{
			level,
			std::string(channel),
			std::string(message)
			});


		if (level == Engine::Log::Level::Fatal)
		{
			// TODO: Message Box や ブレークポイント埋めるなど
		}
	}


}