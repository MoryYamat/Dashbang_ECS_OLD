#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Feature::Character::Animation
{


	class CharacterAnimationFeature
	{
	public:
		//// locomotion animation
		//static void InitProfileDatabase(Engine::ECS::EntityMgr& ecs);
		//static void UpdateBuildAnimationQuery(Engine::ECS::EntityMgr& ecs);
		//static void UpdateMovementAnimationResolver(Engine::ECS::EntityMgr& ecs);
		//static void UpdateAnimationArbiterSystem(Engine::ECS::EntityMgr& ecs);
		//static void UpdateApplyFinalAnimationDecision(Engine::ECS::EntityMgr& ecs);

		//// skill animation
		//static void InitSkillProfileDatabase(Engine::ECS::EntityMgr& ecs);
		//static void UpdateSkillAnimationResolver(Engine::ECS::EntityMgr& ecs);
		//static void UpdateBuildSkillAnimQuerySystem(Engine::ECS::EntityMgr& ecs);


		//// cc animation
		//static void InitCCProfileDatabase(Engine::ECS::EntityMgr& ecs);
		//static void UpdateCCAnimationResolver(Engine::ECS::EntityMgr& ecs);
		//static void UpdateBuildCCAnimQuerySystem(Engine::ECS::EntityMgr& ecs);


		//
		// locomotion animation
		static void InitProfileDatabase(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateBuildAnimationQuery(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateLocmAnimSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		//// skill animation
		//static void InitSkillProfileDatabase(Engine::WorldSystem::Core::WorldCtx& ctx);
		//static void UpdateSkillAnimationResolver(Engine::WorldSystem::Core::WorldCtx& ctx);
		//static void UpdateBuildSkillAnimQuerySystem(Engine::WorldSystem::Core::WorldCtx& ctx);


		//// cc animation
		//static void InitCCProfileDatabase(Engine::WorldSystem::Core::WorldCtx& ctx);
		//static void UpdateCCAnimationResolver(Engine::WorldSystem::Core::WorldCtx& ctx);
		//static void UpdateBuildCCAnimQuerySystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	};


}