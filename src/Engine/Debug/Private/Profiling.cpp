#include "Engine/Debug/Public/ProfilingAPI.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <unordered_map>
#include <sstream>
#include <string>
#include <iomanip>

#if ENGINE_ENABLE_PROFILING

namespace Engine::Debug::Profiling
{
	struct Stat
	{
		double total_us = 0.0;
		std::uint32_t calls = 0;
	};

	static std::unordered_map<std::string, Stat> g_stats;
	//static std::uint32_t g_frameCounter = 0;
	//static std::uint32_t g_printInterval = 60;
	static double g_elapsedSec = 0.0;
	static double g_printIntervalSec = 1.0;

	//void Profiler::SetPrintInterval(std::uint32_t frames)noexcept
	//{
	//	g_printInterval = (frames == 0) ? 1u : frames;
	//}

	void Profiler::SetPrintIntervalSec(double sec) noexcept
	{
		g_printIntervalSec = (sec <= 0.0) ? 1.0 : sec;
	}

	void Profiler::Add(const char* name, double us)
	{
		auto& s = g_stats[std::string(name)];
		s.total_us += us;
		s.calls++;
	}

	void Profiler::EndFrame(double realDt)
	{
		//g_frameCounter++;

		// 
		//if (g_frameCounter % g_printInterval != 0)
		//{
		//	return;
		//}

		if (realDt < 0.0) realDt = 0.0;
		if (realDt > 0.25) realDt = 0.25;

		g_elapsedSec += realDt;

		if (g_elapsedSec < g_printIntervalSec)
			return;

		//// output
		//for (const auto& [name, s] : g_stats)
		//{
		//	if (s.calls == 0) continue;

		//	const double avg_us = s.total_us / static_cast<double>(s.calls);

		//	std::ostringstream oss;
		//	oss << name 
		//		<< " frame=" << g_frameCounter
		//		<< " interval=" << g_printInterval
		//		<< " " << name
		//		<< " calls=" << s.calls
		//		<< " avg=" << std::fixed << std::setprecision(2) << avg_us << "us"
		//		<< " total=" << std::fixed << std::setprecision(2) << s.total_us << "us";

		//	Engine::Log::Write(Engine::Log::Level::Info, "Profiler", oss.str());
		//}

		// output
		for (const auto& [name, s] : g_stats)
		{
			if (s.calls == 0) continue;

			const double avg_us = s.total_us / static_cast<double>(s.calls);

			std::ostringstream oss;
			oss << name
				<< " sec=" << std::fixed << std::setprecision(3) << g_elapsedSec
				<< " calls=" << s.calls
				<< " avg=" << std::fixed << std::setprecision(2) << avg_us << "us"
				<< " total=" << std::fixed << std::setprecision(2) << s.total_us << "us";

			Engine::Log::Write(Engine::Log::Level::Info, "Profiler", oss.str());
		}


		// reset
		g_stats.clear();
		g_elapsedSec = 0.0;
	}
}

#else

namespace Engine::Debug::Profiling
{
	void Profiler::Add(const char*, double) {}
	void Profiler::EndFrame() {}
	// void Profiler::SetPrintInterval(std::uint32_t) noexcept {}
	void Profiler::SetPrintIntervalSec(double)noexcept{}
}

#endif