#include "Game/Character/Control/Public/ControlApi.hpp"
#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"

#include "Game/Character/Control/Public/IntentComponent.hpp"


namespace Game::Character::Control
{
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::Component;
	
	void UpdateCharacterFacingIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = ViewWhere(ctx.rw, All<FacingIntentComponent, Logic2DTransformComponent>{});

		for (const auto& e : ents)
		{
			auto& facing = ctx.ww.Get<FacingIntentComponent>(e);
			auto& logic = ctx.ww.Get<Logic2DTransformComponent>(e);

			logic.front = facing.front;
			logic.rotation = logic.GetRotationYFromFrontVector();
			logic.UpdateRightFromFront();
		}
	}


	void UpdateCharacterMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}


	void UpdateActorPosition(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& fixed = ctx.rw.GetResource<Engine::Time::WorldClockData>().fixedDt;
		auto ents = ViewWhere(ctx.rw, All<Logic2DTransformComponent, Velocity2DComponent>{});

		for (const auto& e : ents)
		{
			auto& logic = ctx.ww.Get<Logic2DTransformComponent>(e);
			auto& vel = ctx.ww.Get<Velocity2DComponent>(e);

			logic.positionXZ += vel.velocity * fixed;
		}
	}

}