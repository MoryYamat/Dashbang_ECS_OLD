#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/ECS/Public/Entity.hpp"

#include "Engine/Component/Private/Common/LifetimeComponent.hpp"

#include <vector>
#include <span>

namespace Game::ECS
{
	// private
	struct LifetimeEntry
	{
		const Engine::ECS::Core::Entity e = Engine::ECS::Core::INVALID;
		Engine::Component::LifetimeComponent* lifetime;
	};

	using LifetimePipeline = std::vector<LifetimeEntry>;

	struct LifetimeSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx_;

		void Update(const std::span<LifetimeEntry> ents, const float dt);
	};


	// public
	void UpdateLifetimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}