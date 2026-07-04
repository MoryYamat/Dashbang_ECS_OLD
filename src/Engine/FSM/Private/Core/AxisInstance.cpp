#include "Engine/FSM/Public/Core/AxisInstance.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/Analysis.hpp"

#include "Engine/Debug/Public/ProfilingAPI.hpp"

#include <algorithm>
#include <iostream>

namespace Engine::FSM::Core
{
	//static inline bool CellHasEdges(const CanonicalFSM& f, std::uint32_t st, std::uint32_t slot)
	//{
	//	const auto S = f.numSlots;
	//	const auto cell = st * S + slot;
	//	return f.ofs[cell] < f.ofs[cell + 1];
	//}


	bool AxisInstance::ApplyDecision(const Decision& d)
	{
		if (!d.changed) return false;
		prevState = curState;
		curState = d.to;
		prevProfile = curProfile;

		//std::cout << "[ApplyDecision]: " << prevState << " changed to " << curState << "\n";
		//// std::cout<<"[ApplyDecision]: "

		return true;
	}


	Decision AxisInstance::tick(const IFieldReader& reader)
	{
		assert(ax && fsm && plan && eval);
		// 計測
		// ::Engine::Debug::Profiling::Profiler::SetPrintInterval(1);
		//::Engine::Debug::Profiling::ScopedTimer timer("FSM::AxisInstance::tick");

		Decision d{};

		d.from = curState;
		d.to = curState;
		d.changed = false;
		d.prio = 0;
		d.slot = UINT32_MAX;

		// 必要な派生ビットだけ実行( Derived チャネルに反映)
		auto& derived = conds.ch[(size_t)CondChannel::Derived];
		// derived.clearAll();
		ExecuteDerivedCondsFiltered(*plan, requiredBits, reader, derived);

		// 4チャネル合成 → composed
		BitEnvSnapshot composed;
		conds.compose(composed, static_cast<std::uint32_t>(ax->condOrder.size()));

		if (fsm->alwaysTrueBit != UINT32_MAX)// TODO: condIdx = 0 に固定すれば、if文を抹消できる(予約)
		{
			composed.set(fsm->alwaysTrueBit, true);
		}

		// 候補スロットを優先度でチェック(BuildPlanForで既に安定ソート済み)
		const auto S = fsm->numSlots;
		for (auto s : candidateSlots)
		{
			// slot に割り当てられている条件(profile差替え済み)
			const CondID cid = fsm->condOf[curProfile * S + s];
			if (cid.valid())
			{
				// 必須ビットが立っていなければ不採用
				if (!composed.testCondBit(cid.v)) continue;
			}

			// 条件を満たした最初の slot が「選ばれたslot」
			const std::uint32_t cell = curState * S + s;
			if (fsm->ofs[cell] == fsm->ofs[cell + 1]) continue;

			const std::uint32_t a = fsm->ofs[cell];

			// edges[a]が prio 最大かつ toIdx 最小 (CSR 構築で安定ソート)
			const TransitionEdge& e = fsm->edges[a];

			d.to = e.toIdx;
			d.changed = (d.to != d.from);
			d.prio = e.prio;
			d.slot = s;
			return d;
		}

		// 遷移なし
		return d;
	}
}// namespace Engine::FSM::Core



// // エンティティ初期化時（Movement例）
// auto& mov = ww.Add<MovementAxisComponent>(e);
// 
// // 参照の取り出し
// const auto& cat = ww.GetResource<FSMCatalog>();
// auto* rdb = ww.TryGetResource<AxisRuntimeDB>();
// const CanonicalAxis& ax = cat.axes[cat.axes[0].axisName == "Movement" ? 0 : 0]; // findでOK
// const CanonicalFSM& fsm = ax.fsms[ax.fsmIdxByName.at("Basic")];
// const EnvAssemblerPlan& plan = rdb->get("Movement")->plan;
// 
// // ★ 当面はここで個別に前計算（後で共有に置き換え可）
// PrecomputedEvalPlans plans = BuildEvalPlans(fsm);
// 
// mov.inst.init(ax, fsm, plan, plans,
// 	ax.stateIdxByName.at("Idle"),
// 	ax.profileIdxByName.at("Default"));


// 毎フレーム
// mov.inst.beginFrame();              // 必要なら Latched 寿命など
// mov.reader.movementInputMag = ComputeInputMagnitude(e);
// 
// // Decide
// Decision d = mov.inst.tick(mov.reader);
// if (d.changed) {
// 	// ここで OnExit/OnEnter など（必要なら）
// 	// mov.inst.stateLocal = d.to; ← 状態を採用する責務は呼び出し側で明示に
// 	const_cast<Engine::FSM::Core::AxisInstance&>(mov.inst).stateLocal = d.to;
// }
