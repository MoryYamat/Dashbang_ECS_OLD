#include "WorldClock.hpp"

#include "Engine/WorldSystem/Private/Core/WorldCtx.hpp"
#include "Engine/WorldSystem/Private/Core/WorldAccess.hpp"

#include <algorithm>
#include <cmath>

namespace Engine::Time
{
	using namespace Engine::ECS;

	void WorldClockSystem::InitWorldClock(EntityMgr& ecs)
	{
		auto& clock = ecs.createResource<WorldClockData>();
	}

	void WorldClockSystem::TickWorldClock(EntityMgr& ecs, float deltaTime)
	{
		auto& clock = ecs.getOrCreateResource<WorldClockData>();
		clock.dt = clock.paused ? 0.0f : deltaTime * clock.scale;
		clock.now += clock.dt;
	}

	// world ver.
	void WorldClockSystem::InitWorldClock(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& clock = ctx.ww.CreateResource<WorldClockData>();
	}


	void WorldClockSystem::BeginFrame(Engine::WorldSystem::Core::WorldCtx& ctx, float realDt)
	{
		auto& wc = ctx.ww.GetResource<WorldClockData>();
		const float maxFrameDt = 1.f / 15.f;// 0.0666...
		wc.frameDt = std::min(realDt * wc.timeScale, maxFrameDt);// 
		wc.accumulator += wc.frameDt;
		wc.stepsThisFrame = 0;
	}

	bool WorldClockSystem::PopFixedStep(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& wc = ctx.ww.GetResource<WorldClockData>();
		constexpr int MAX_CATCHUP_STEPS = 8;
		if (wc.accumulator < wc.fixedDt) return false;
		if (wc.stepsThisFrame >= MAX_CATCHUP_STEPS)
		{
			wc.accumulator = std::fmod(wc.accumulator, wc.fixedDt);
			return false;
		}

		wc.accumulator -= wc.fixedDt;
		wc.tick++;
		wc.stepsThisFrame++;
		return true;
	}

	void WorldClockSystem::EndFrame(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& wc = ctx.ww.GetResource<WorldClockData>();
		wc.alpha = wc.accumulator / wc.fixedDt;// [0,1)

		// 計測用
		// --- 追加 ---
		//static int   frameCount = 0;
		//static int   sumSteps = 0;
		//static float sumTime = 0.0f;

		//frameCount++;
		//sumSteps += wc.stepsThisFrame;
		//sumTime += wc.frameDt;

		//if (frameCount == 60)
		//{
		//	std::cerr
		//		<< "[SUM 60 frames] "
		//		<< "time=" << sumTime << "s "
		//		<< "fixedSteps=" << sumSteps
		//		<< " avgStepsPerFrame=" << (sumSteps / 60.0f)
		//		<< "\n";

		//	frameCount = 0;
		//	sumSteps = 0;
		//	sumTime = 0.0f;
		//}


		wc.frame++;
	}
}