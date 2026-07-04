//
#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"



namespace Game::Feature::Character
{
	class StateFeature
	{
	public:
		static void InitTransitionDatabase(Engine::ECS::EntityMgr& ecs);

		static void UpdateCharacterState(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void UPdateCharacterSkillExecutionState(Engine::ECS::EntityMgr& ecs, float deltaTime);


		//
		static void InitTransitionDatabase(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UpdateCharacterState(Engine::WorldSystem::Core::WorldCtx& ctx);

		static void UPdateCharacterSkillExecutionState(Engine::WorldSystem::Core::WorldCtx& ctx);
	};
}