#pragma once

#include "Engine/FSM/Public/FSMApi.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Character/FSM/Public/FSMApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/FSM/Public/MovementTypes.hpp"
#include "Game/Character/FSM/Public/MovementAxisApi.hpp"

#include "Game/Character/FSM/Public/SkillAxisApi.hpp"


#include "Engine/Log/Public/LogApi.hpp"

#include <string_view>

namespace Game::FSM
{
	inline Engine::FSM::Core::RegisterFn MakeGameRegisterProvider()
	{
		return [](Engine::FSM::Core::FSMRegistry& reg)
			{
				Game::Character::FSM::Movement::RegisterMovementAxes(reg);
				Game::Character::FSM::Skill::RegisterSkillAxes(reg);
			};
	}

	inline Engine::FSM::Core::FieldResolverProvider MakeGameResolverProvider()
	{
		return [](std::string_view axisName, Engine::FSM::Core::FieldResolver& out)->bool
			{
				if (axisName == "Movement") { out = Game::Character::FSM::Movement::MakeMovementFieldResolver(); return true; }
				// 他 Axis はここに追加("Skill" など)
				if (axisName == "Skill") { out = Game::Character::FSM::Skill::MakeSkillFieldResolver(); return true; }
				return false;// 未登録Axis
			};

	}

	inline bool InitGameTable(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		using namespace Game::Character::FSM::Movement;
		using namespace Game::Character::FSM::Skill;

		bool ok = true;

		// TODO: API化
		{
			auto& cTbl = ctx.ww.CreateResource<MovementCondTable>();
			auto& sTbl = ctx.ww.CreateResource<MovementStateTable>();
			auto& pTbl = ctx.ww.CreateResource<MovementProfileTable>();
			auto& lTbl = ctx.ww.CreateResource<MovementLogicTable>();
			if (!InitMovementTable(ctx, cTbl, sTbl, pTbl, lTbl))
			{
				std::printf("[InitGameCondTable] Failed to init MovementTable\n");
				ok = false;
			}
		}

		{
			auto& cTble = ctx.ww.CreateResource<SkillCondTable>();
			auto& sTble = ctx.ww.CreateResource<SkillStateTable>();
			auto& pTble = ctx.ww.CreateResource<SkillProfileTable>();
			if (!InitSkillFSMTable(ctx, cTble, sTble, pTble))
			{
				Engine::Log::Write(Engine::Log::Level::Error, "InitSkillFSMTable", "Failed to init SkillFSMTable");
				ok = false;
			}
		}

		// 以下にほかの軸も追加していく


		return ok;
	}
}