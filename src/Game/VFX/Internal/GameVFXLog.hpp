#pragma once

#include "Engine/Log/Public/LogApi.hpp"

namespace Game::VFX::Log
{
	using namespace Engine::Log;

	inline constexpr const char* kGameVFXCatalogBuilder = "GameVFXCatalogBuilder";
	inline constexpr const char* kInitAllGameVFXSystem = "InitAllGameVFXSystem";

	inline void error(const char* tag, const char* msg)
	{
		Write(Level::Error, tag, msg);
	}

	inline void warn(const char* tag, const char* msg)
	{
		Write(Level::Warn, tag, msg);
	}

	inline void fatal(const char* tag, const char* msg)
	{
		Write(Level::Fatal, tag, msg);
	}

	inline void info(const char* tag, const char* msg)
	{
		Write(Level::Info, tag, msg);
	}

	inline void debug(const char* tag, const char* msg)
	{
		Write(Level::Debug, tag, msg);
	}

	inline void trace(const char* tag, const char* msg)
	{
		Write(Level::Trace, tag, msg);
	}
}