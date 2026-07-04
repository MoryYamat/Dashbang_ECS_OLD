#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"
#include "Engine/Debug/Public/DebugFwd.hpp"

namespace Game::Feature::Debug
{
	class DebugFeature
	{
	public:
		static void SpawnDummyBatch(::Engine::WorldSystem::Core::WorldCtx& ctx, int count);
		static void InitDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateDummySystem(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateDebugDrawSystem(Engine::WorldSystem::Core::WorldCtx& ctx,
			const Engine::Graphics::RenderContext& rc,
			Engine::Debug::DebugLineRenderer& lineRenderer);
	private:

	};
}