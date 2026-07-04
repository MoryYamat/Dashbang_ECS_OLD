#include "Game/00_Feature/Private/Debug/DebugFeatureAPI.hpp"


#include "Engine/Debug/Public/DebugDrawAPI.hpp"
#include "Game/Combat/HitBox/Public/HitBoxAPI.hpp"

#include "Game/Actor/Public/TestDummyActor.hpp"
#include "Game/Debug/Public/DebugApi.hpp"

namespace Game::Feature::Debug
{
	void DebugFeature::SpawnDummyBatch(::Engine::WorldSystem::Core::WorldCtx& ctx, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			::Game::Actor::Debug::TestFSMDummyActor::TestFSMDummyActor(ctx);
		}
	}

	void DebugFeature::InitDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Engine::Debug::InitAllDebugDrawSystem(ctx);
	}

	void DebugFeature::UpdateDummySystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		::Game::Debug::FSM::TestFSMDummyDriverSystem(ctx);
	}

	void DebugFeature::UpdateDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Graphics::RenderContext& rc,
		Engine::Debug::DebugLineRenderer& lineRenderer)
	{
		Game::Combat::HitBox::UpdateAllHitBoxDebugSystem(ctx);
		Engine::Debug::UpdateAllDebugDrawSystem(ctx, rc, lineRenderer);
	}
}