#pragma once

#include "MovementAxisComponent.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

//?
#include "Game/Character/FSM/Public/MovementTypes.hpp"// 前方宣言ではだめか
//?
#include <string_view>

namespace Game::Character::FSM::Movement
{

	bool InitMovementAxis(Engine::WorldSystem::Core::WorldCtx& ctx, MovementAxisComp& comp);
	bool InitMovementAxis(Engine::WorldSystem::Core::WorldCtx& ctx, MovementAxisComp& comp, std::string_view fsmName);

	// void makeAxis_Movement(Engine::FSM::Core::FSMRegistry& reg);
	// void makeFSM_Movement(Engine::FSM::Core::FSMRegistry& reg);
	void RegisterMovementAxes(Engine::FSM::Core::FSMRegistry& reg);
	
	// void BuildMovementPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, MovementPipeline& out);

	void UpdateMovementAll(Engine::WorldSystem::Core::WorldCtx& ctx);

	// void ExecuteMovementOps(std::uint32_t mask, Engine::WorldSystem::Core::WorldCtx& ctx, const MovementPipelineEntry& ent, float dt);
	void SetMovementVelComp(const MovementPipelineEntry& entry, Engine::WorldSystem::Core::WorldCtx& ctx);
	void SetZeroVel(const MovementPipelineEntry& entry, Engine::WorldSystem::Core::WorldCtx& ctx);

	bool InitMovementCondTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementCondTable& out);
	bool InitMovementStateTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementStateTable& out);
	bool InitMovementProfTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementProfileTable& out);
	bool InitMovementLogicTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementLogicTable& out);
	bool InitMovementTable(Engine::WorldSystem::Core::WorldCtx& ctx,
		MovementCondTable& ctbl, MovementStateTable& stbl, MovementProfileTable& ptbl, MovementLogicTable& ltbl);
}

// 責務分離
// Input->Intent->Resolver(FSM) -> State -> Logic -> Anim/Draw のフローを成立させる
// 
// Resolver(FSM): Input/Intent/World状態->CondBank の状態を変更 (MovementEnvSystem)
// 
// Resolver(State): →事前定義されたFSMの状態(State)を確定→ResolverのState を StateComponentに映す (MovementFSMSystem)
// 
// Logic: State の状態に定義された動作を実行し、ECSのデータを更新する (MovementLogicSystem)
//