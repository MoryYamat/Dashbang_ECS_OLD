#pragma once

#include "Engine/Log/Public/LogApi.hpp"

namespace Engine::Audio::Log
{
	inline constexpr const char* kCatalog = "AudioCatalog";
	inline constexpr const char* kSystem = "AudioSystem";
	inline constexpr const char* kBackend = "AudioBackend";

	using namespace ::Engine::Log;

	inline void debug(const char* tag, const char* msg)
	{
		Write(Level::Debug, tag, msg);
	}

	inline void info(const char* tag, const char* msg)
	{
		Write(Level::Info, tag, msg);
	}

	inline void warn(const char* tag, const char* msg)
	{
		Write(Level::Warn, tag, msg);
	}

	inline void error(const char* tag, const char* msg)
	{
		Write(Level::Error, tag, msg);
	}
}