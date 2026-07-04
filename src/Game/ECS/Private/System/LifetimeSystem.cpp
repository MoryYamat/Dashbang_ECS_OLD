#include "Game/ECS/Public/GameECSAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/ECS/Public/Entity.hpp"

#include "Engine/Component/Private/Common/LifetimeComponent.hpp"

#include "Engine/Log/Public/LogAPI.hpp"

#include <vector>
#include <iostream>

namespace
{
	using namespace Game::ECS;
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::Component;

	void BuildLifetimePipeline(Engine::WorldSystem::Core::WorldCtx& ctx, LifetimePipeline& out)
	{
		out.clear();
		auto ents = ViewWhere(ctx.rw, All<LifetimeComponent>{});

		for (const auto& e : ents)
		{
			auto* lifetime = ctx.ww.TryGet<LifetimeComponent>(e);

			if (!lifetime)// 理論上ないはずだが、念のため
			{
				Engine::Log::Write(Engine::Log::Level::Fatal, "LifetimeSystem", "Failed to obtain LifetimeComponent.");
				continue;
			}

			out.push_back(LifetimeEntry{
				.e = e,
				.lifetime = lifetime
				});
		}
	}
}

namespace Game::ECS
{
	void LifetimeSystem::Update(const std::span<LifetimeEntry> ents, const float dt)
	{
		std::vector<Engine::ECS::Core::Entity> toDestroy;

		for (const auto& e : ents)
		{
			if (!e.lifetime)
				continue;		// defensive

			e.lifetime->remainingLifetime -= dt;
			if (e.lifetime->remainingLifetime <= 0.f)
			{
				toDestroy.push_back(e.e);
			}
		}


		for (auto e : toDestroy)
		{
			// std::cout << "here\n";
			ctx_.ww.Destroy(e);
		}
	}


	void UpdateLifetimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& clock = ctx.rw.GetResource<Engine::Time::WorldClockData>();
		LifetimePipeline pipeline;
		BuildLifetimePipeline(ctx, pipeline);

		LifetimeSystem lifeSys{ ctx };
		lifeSys.Update(pipeline, clock.fixedDt);
	}
}
