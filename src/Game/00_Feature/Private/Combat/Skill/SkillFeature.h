// 

#pragma once

#include "Engine/ECS/EntityManager.h"



#include "Engine/WorldSystem/Public/WorldFwd.hpp"



namespace Game::Feature::Combat
{
	class SkillFeature
	{
	public:
		static void InitSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void UpdateSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

		// Skill 関連システムのすべての初期化後に呼び出す
		static void InitSkillBindingSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

}