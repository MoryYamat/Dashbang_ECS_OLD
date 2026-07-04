#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"// 依存関係の検討必要
#include "Game/Character/FSM/Public/SkillTypes.hpp"// 依存関係の検討必要


#include <string_view>

namespace Game::Character::FSM::Skill
{
	bool InitSkillAxis(Engine::WorldSystem::Core::WorldCtx& ctx, SkillAxisComp& comp);
	bool InitSkillAxis(Engine::WorldSystem::Core::WorldCtx& ctx, SkillAxisComp& comp, std::string_view fsmName);

	bool InitSkillFSMTable(Engine::WorldSystem::Core::WorldCtx& ctx,
		SkillCondTable& ctbl, SkillStateTable& stbl, SkillProfileTable& ptbl);

	void RegisterSkillAxes(Engine::FSM::Core::FSMRegistry& reg);

	void UpdateAllSkillFSM(Engine::WorldSystem::Core::WorldCtx& ctx);

	// internal
	// void BuildSkillPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, SkillPipeline& out);

}