// Skill Core ← Binding (HitBox / Anim / Sound / ... )
// 各機能をSkillCore に付与して Skill定義を完成させる
#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Combat/Skill/Public/CoreFwd.hpp"

namespace Game::Combat::Skill::Binding
{
	// private
	void RegisterSkillBindingDTO(Game::Combat::Skill::Binding::SkillBindingBuilder& builder);


	// pubilc
	void InitAllSkillBindingSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}