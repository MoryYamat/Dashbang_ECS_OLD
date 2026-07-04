#pragma once
#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


namespace Game::Feature::Character::Skill::FSM
{
	class SkillFSMFeature
	{
	public:
		static void UpdateSkillFSMSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillFSMResolverSystem(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UpdateSkillInterferenceResolver(Engine::ECS::EntityMgr& ecs);




		static void UpdateSkillFSMSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateSkillFSMResolverSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateSkillInterferenceResolver(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}