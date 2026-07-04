#include "Engine/Log/Public/LogApi.hpp"

#include <cstdio>

namespace Engine::Log
{
	static ILogger* g_logger = nullptr;

	void SetLogger(ILogger* logger) noexcept
	{
		g_logger = logger;
	}

	ILogger* GetLogger() noexcept
	{
		return g_logger;
	}

	void Write(Level level, std::string_view channel, std::string_view message)
	{
		if (g_logger)
		{
			g_logger->Write(level, channel, message);
		}
		else
		{
			// フォールバック
			std::fprintf(stderr,
				"[Engine][%.*s][%u] %.*s\n",
				(int)channel.size(), channel.data(),
				static_cast<unsigned>(level),
				(int)message.size(), message.data());
		}
	}
}