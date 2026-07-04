#pragma once


#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Character::Animation
{
	void UpdateAllLocomResolverSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	struct ApplyFinalAnimationDecisionSystem
	{
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	struct AnimationArbiterSystem
	{
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);

	};

}

namespace Game::Character::Animation::Movement
{
	void LocomQuerySystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	// movement
	struct MovementAnimationResolver
	{
		static void Update(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}

namespace Game::Character::Animation::Skill
{
	struct SkillAnimRequestLifetimeSystem
	{
		::Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(const float dt);
	};
}