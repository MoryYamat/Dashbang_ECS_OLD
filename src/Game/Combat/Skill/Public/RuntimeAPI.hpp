#pragma once
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Character/FSM/Public/FSMFwd.hpp"
#include "Game/Character/Control/Public/ControlFwd.hpp"

#include "Game/Combat/Skill/Public/CoreFwd.hpp"

#include <span>

namespace Game::Combat::Skill::Runtime
{


	// public
	void InitAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
	void UpdateAllSkillRuntimeSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}