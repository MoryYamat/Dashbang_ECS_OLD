#pragma once

#include "Engine/Log/Public/LogApi.hpp"

namespace Game::Audio::Log
{
	using namespace ::Engine::Log;

	inline constexpr const char* kChannel = "Game.Audio";

	inline void debug(const char* msg)
	{
		Write(::Engine::Log::Level::Debug, kChannel, msg);
	}

	inline void info(const char* msg)
	{
		Write(::Engine::Log::Level::Info, kChannel, msg);
	}

	inline void error(const char* msg)
	{
		Write(::Engine::Log::Level::Error, kChannel, msg);
	}

	inline void warn(const char* msg)
	{
		Write(::Engine::Log::Level::Warn, kChannel, msg);
	}

	inline void fatal(const char* msg)
	{
		Write(::Engine::Log::Level::Fatal, kChannel, msg);
	}
}