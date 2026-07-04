#pragma once

#include "Engine/Log/Public/LogApi.hpp"

#include <vector>
#include <string>
#include <string_view>

namespace Game::Debug
{
	struct LogEntry
	{
		Engine::Log::Level level;
		std::string			channel;
		std::string			message;
	};

	class GameLogger : public Engine::Log::ILogger
	{
	public:
		void Write(Engine::Log::Level level,
			std::string_view channel,
			std::string_view message) override;

		const std::vector<LogEntry>& GetEntries() const noexcept { return entries_; }

	private:
		std::vector<LogEntry> entries_;
	};

}