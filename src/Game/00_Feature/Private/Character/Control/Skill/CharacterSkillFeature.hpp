#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


namespace Game::Feature::Character
{
	// Character Skill Feature
	class CharacterSkillFeature
	{
	public:
		//static void UpdateCharacterSkillExecution(Engine::ECS::EntityMgr& ecs, float deltaTime);
		//
		//static void UpdateCharacterSkillIntentResolver(Engine::ECS::EntityMgr& ecs);

		static void UpdateCharacterSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx);

		//static void UpdateCharacterSkillExecution(Engine::WorldSystem::Core::WorldCtx& ctx);

		//static void UpdateCharacterSkillIntentResolver(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
} // namespace Game::Character::Control::Skill