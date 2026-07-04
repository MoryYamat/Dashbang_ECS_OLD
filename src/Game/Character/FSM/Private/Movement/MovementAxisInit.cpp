#include "Game/Character/FSM/Public/MovementAxisApi.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/InitAxisComponent.hpp"

#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Core;
	using namespace Engine::WorldSystem::Core;

	bool InitMovementAxis(WorldCtx& ctx, MovementAxisComp& comp)
	{

		return InitAxisComponent<MovementTag>(ctx, comp);
	}

	bool InitMovementAxis(WorldCtx& ctx, MovementAxisComp& comp, std::string_view fsmName)
	{
		return InitAxisComponent<MovementTag>(ctx, comp, fsmName);
	}

	// Movement軸内の固有cond名の別名をデータベースに登録
	bool InitMovementCondTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementCondTable& out)
	{
		auto* cat = ctx.ww.TryGetResource<FSMCatalog>();
		if (!cat) return false;

		const CanonicalAxis* movementAxis = nullptr;
		for (auto& ax: cat->axes)
		{
			if (ax.axisName == "Movement")
			{
				movementAxis = &ax;
				break;
			}
		}
		if (!movementAxis)return false;

		const auto& condMap = movementAxis->condIdxByName;

		auto findBit = [&](const char* name, std::uint32_t& dst) -> bool
			{
				auto it = condMap.find(name);
				if (it == condMap.end()) return false;
				dst = it->second;
				return true;
			};


		bool ok = true;

		// ok &= findBit("canMove", out.canMove);
		// ok &= findBit("shouldStop", out.shouldStop);


		return ok;
	}

	bool InitMovementStateTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementStateTable& out)
	{
		auto* cat = ctx.ww.TryGetResource<FSMCatalog>();
		if (!cat) return false;

		const CanonicalAxis* movementAxis = nullptr;
		for (auto& ax : cat->axes)
		{
			if (ax.axisName == "Movement")
			{
				movementAxis = &ax;
				break;
			}
		}
		if (!movementAxis)return false;

		const auto& stateMap = movementAxis->stateIdxByName;

		auto findBit = [&](const char* name, std::uint32_t& dst) -> bool
			{
				auto it = stateMap.find(name);
				if (it == stateMap.end()) return false;
				dst = it->second;
				return true;
			};


		bool ok = true;

		ok &= findBit("Idle", out.Idle.v);
		ok &= findBit("Moving", out.Moving.v);


		return ok;
	}

	bool InitMovementProfTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementProfileTable& out)
	{
		auto* cat = ctx.ww.TryGetResource<FSMCatalog>();
		if (!cat) return false;

		const CanonicalAxis* movementAxis = nullptr;
		for (auto& ax : cat->axes)
		{
			if (ax.axisName == "Movement")
			{
				movementAxis = &ax;
				break;
			}
		}
		if (!movementAxis)return false;

		const auto& profMap = movementAxis->profileIdxByName;

		auto findBit = [&](const char* name, std::uint32_t& dst) -> bool
			{
				auto it = profMap.find(name);
				if (it == profMap.end()) return false;
				dst = it->second;
				return true;
			};


		bool ok = true;

		ok &= findBit("Default", out.Default.v);


		return ok;
	}

	bool InitMovementLogicTable(Engine::WorldSystem::Core::WorldCtx& ctx, MovementLogicTable& out)
	{
		auto* stbl = ctx.ww.TryGetResource<MovementStateTable>();
		auto* ptbl = ctx.ww.TryGetResource<MovementProfileTable>();
		if (!stbl || !ptbl) return false;

		out.entries.clear();

		// TODO: 後でDTO/YAMLなど で定義できるようにする
		// Moving の間: 速度適用
		{
			MovementLogicEntry e{};
			e.key.state = stbl->Moving;
			e.key.profile = ptbl->Default;
			e.key.trigger = StateTriggerKind::WhileIn;
			e.opMask |= OpBit(MovementOpKind::ApplyVelocityFromIntent);
			out.entries.push_back(e);
		}

		// Idle の間: 速度0
		{
			MovementLogicEntry e{};
			e.key.state = stbl->Idle;
			e.key.profile = ptbl->Default;
			e.key.trigger = StateTriggerKind::OnEnter;
			e.opMask |= OpBit(MovementOpKind::ZeroVelocity);
			out.entries.push_back(e);
		}


		return true;
	}

	bool InitMovementTable(Engine::WorldSystem::Core::WorldCtx& ctx,
		MovementCondTable& ctbl, MovementStateTable& stbl, MovementProfileTable& ptbl, MovementLogicTable& ltbl)
	{
		bool ok = InitMovementCondTable(ctx, ctbl);
		ok &=InitMovementStateTable(ctx, stbl);
		ok &= InitMovementProfTable(ctx, ptbl);
		ok &= InitMovementLogicTable(ctx, ltbl);

		return ok;
	}
}