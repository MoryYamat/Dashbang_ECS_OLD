#pragma once

#include "Engine/Public/EngineFwd.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"

namespace Game::Character::FSM::Movement
{
	//void RegisterMovementAuthoring(Engine::FSM::Core::FSMRegistry& reg);

	// void BuildMovementCondTable_Stage(Engine::FSM::Core::CondTableStaging& stage);

	// void SetupMovementAxisRuntime(Engine::WorldSystem::Core::WorldCtx& ctx);

	// void makeAxis_Movement(Engine::FSM::Core::FSMRegistry& reg);
	// void makeFSM_Movement(Engine::FSM::Core::FSMRegistry& reg);
	// void RegisterMovementAxes(Engine::FSM::Core::FSMRegistry& reg);
	Engine::FSM::Core::FieldResolver MakeMovementFieldResolver();
	//void ProvideMovementConds(const Engine::FSM::Core::FSMCatalog& cat,
	//	std::vector<Engine::FSM::Core::AxisCondBindings>& out);
	// Engine::FSM::Core::FieldResolver MakeMovementFieldResolver();

}

namespace Game::Character::FSM::Skill
{
	Engine::FSM::Core::FieldResolver MakeSkillFieldResolver();
}