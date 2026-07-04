#include "SkillFeature.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


#include "Game/Combat/Skill/Public/CoreApi.hpp"
#include "Game/Combat/Skill/Public/BindingAPI.hpp"
#include "Game/Combat/Skill/Public/RuntimeAPI.hpp"

//
//
namespace Game::Feature::Combat
{
	void SkillFeature::InitSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::InitAllSkillSystem(ctx);
	}

	void SkillFeature::UpdateSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::Runtime::UpdateAllSkillRuntimeSystem(ctx);
	}

	void SkillFeature::InitSkillBindingSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::Binding::InitAllSkillBindingSystem(ctx);
	}
}