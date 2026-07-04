// Movement Axis DTO
#include "Game/Character/FSM/Public/MovementAxisApi.hpp"

#include "Engine/FSM/Public/Core/DTO.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Core/Registry.hpp"

#include <unordered_map>



namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Core;

	static void makeAxis_Movement(FSMRegistry& reg)
	{
		AxisDTO ax;
		ax.axis = "Movement";
		ax.stateU = {"Idle", "Moving" };
		ax.condU = { "canMove", "shouldStop", "AlwaysTrue" };
		ax.slotU = { "Transition", "Stop" };
		ax.profileU = { "Default" };
		ax.condDefs = {
			{.cond = "canMove",    .kind = "CompareF", .field = "movementInputMag", .op = ">",  .th = "0.00" },
			{.cond = "shouldStop",    .kind = "CompareF", .field = "movementInputMag", .op = "<=",  .th = "0.00" },
		};
		ax.version = 1;
		reg.add(std::move(ax));
	}

	static void makeFSM_Movement(FSMRegistry& reg)
	{
		FSMDTO f;
		f.axis = "Movement";
		f.fsm = "Basic";
		f.states = { "Idle", "Moving" };
		f.profiles = { "Default" };

		// profile defs
		f.profile_defs.push_back(ProfileDefDTO{
			.name = "Default",
			.extends = "",
			.binds = {
				ProfileBindDTO{
				.slot = "Transition",
				.cond = "canMove",
			},
				ProfileBindDTO{
				.slot = "Stop",
				.cond = "shouldStop",
			}
			}
			});

		// transitions
		f.transitions.push_back({ "Idle", "Moving", "Transition", 0 });
		f.transitions.push_back({ "Moving", "Idle", "Stop", 0 });

		f.version = 1;

		reg.add(std::move(f));
	}

	void RegisterMovementAxes(Engine::FSM::Core::FSMRegistry& reg)
	{
		makeAxis_Movement(reg);
		makeFSM_Movement(reg);
	}
}


// AxisDTO ax;
// ax.axis = "Movement";
// ax.stateU = { "Idle", "Moving" };
// ax.condU = { "canMove", "shouldStop" };
// ax.slotU = { "Transition", "Stop" };
// ax.profileU = { "Default" };
// 
// // ここで cond の意味とパラメータを Axis に集約定義
// ax.condDefs = {
// 	{.cond = "canMove",    .kind = "CompareF", .field = "moveInputMag", .op = ">",  .th = "0.25" },
// 	{.cond = "shouldStop", .kind = "CompareF", .field = "moveInputMag", .op = "<=", .th = "0.01" },
// };

// f.profile_defs = {
//   {.name = "Default", .binds = {
// 	  {.slot = "Transition", .cond = "canMove" },
// 	  {.slot = "Stop",       .cond = "shouldStop" }
//   }}
// };
