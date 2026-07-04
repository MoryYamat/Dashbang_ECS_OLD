#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"

#include "Engine/FSM/Public/Core/DTO.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Core/Registry.hpp"

#include <unordered_map>

#include <iostream>

namespace Game::Character::FSM::Skill
{
	using namespace Engine::FSM::Core;

	static void makeAxis_Skill(FSMRegistry& reg)
	{
		AxisDTO ax;
		ax.axis = "Skill";
		ax.stateU = { "None", "Casting", "Active", "Recovery", "Completed"};
		ax.condU = { "Triggered","TimeElapsed", "AlwaysTrue"};
		ax.slotU = { "Triggered", "PhaseAdvance", "Reset" };
		ax.profileU = { "Default" };
		ax.condDefs = { 
			{.cond = "Triggered", .kind = "Bit"},
			{.cond = "TimeElapsed", .kind = "Bit"},
		};
		ax.version = 1;
		reg.add(std::move(ax));
	}

	static void makeFSM_Skill(FSMRegistry& reg)
	{
		FSMDTO f;
		f.axis = "Skill";
		f.fsm = "Basic";
		f.states = { "None", "Casting", "Active", "Recovery", "Completed" };
		f.profiles = { "Default" };

		f.profile_defs.push_back(ProfileDefDTO{
			.name = "Default",
			.extends = "",
			.binds = {
				ProfileBindDTO{.slot = "Triggered", .cond = "Triggered"},
				ProfileBindDTO{.slot = "PhaseAdvance", .cond = "TimeElapsed"},
				ProfileBindDTO{.slot = "Reset", .cond = "AlwaysTrue"},
			}
			});

		f.transitions.push_back({ "None", "Casting", "Triggered", 0 });
		f.transitions.push_back({ "Casting", "Active", "PhaseAdvance", 0 });
		f.transitions.push_back({ "Active", "Recovery", "PhaseAdvance", 0 });
		f.transitions.push_back({ "Recovery", "Completed", "PhaseAdvance", 0 });
		f.transitions.push_back({ "Completed", "None", "Reset", 0 });

		f.version = 1;

		reg.add(std::move(f));
	}

	void RegisterSkillAxes(Engine::FSM::Core::FSMRegistry& reg)
	{
		makeAxis_Skill(reg);
		makeFSM_Skill(reg);
	}
}