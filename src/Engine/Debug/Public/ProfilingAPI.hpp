#pragma once

#ifndef ENGINE_ENABLE_PROFILING
#define ENGINE_ENABLE_PROFILING 0
#endif


#include <cstdint>

namespace Engine::Debug::Profiling
{
	class Profiler
	{
	public:
		// call from `ScopedTimer`
		static void Add(const char* name, double us);

		// log and reset
		static void EndFrame(double realDt);

		// Output frame interval
		// static void SetPrintInterval(std::uint32_t frames) noexcept;
		static void SetPrintIntervalSec(double sec) noexcept;

	private:
		Profiler() = delete;
	};


}


#if ENGINE_ENABLE_PROFILING

#include <chrono>

namespace Engine::Debug::Profiling
{
	struct ScopedTimer
	{
		const char* name;
		std::chrono::high_resolution_clock::time_point start;

		explicit ScopedTimer(const char* n)
			: name(n), start(std::chrono::high_resolution_clock::now()) {
		}

		~ScopedTimer()
		{
			auto end = std::chrono::high_resolution_clock::now();
			double us = std::chrono::duration<double, std::micro>(end - start).count();
			Profiler::Add(name, us);
		}
	};
}

#else	// ENGINE_ENABLE_PROFILING

// OFF
namespace Engine::Debug::Profiling
{
	struct ScopedTimer
	{
		explicit ScopedTimer(const char*){}
	};
}

#endif