#pragma once

#include <cstdint>
#include <string_view>

namespace Engine::Log
{
	enum class Level : std::uint8_t
	{
		Trace,
		Debug,
		Info,
		Warn,
		Error,
		Fatal,
	};

	struct ILogger
	{
		virtual ~ILogger() = default;
		virtual void Write(
			Level level,
			std::string_view channel,
			std::string_view message
		) = 0;
	};

	void SetLogger(ILogger* logger) noexcept;
	ILogger* GetLogger() noexcept;
	void Write(Level level, std::string_view channel, std::string_view message);
}