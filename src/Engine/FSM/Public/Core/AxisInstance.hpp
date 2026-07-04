#pragma once

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/Analysis.hpp"
#include "Engine/FSM/Public/Core/ConditionBank.hpp"

#include <cstdint>
#include <span>
#include <cassert>

namespace Engine::FSM::Core
{
	struct AxisInstance
	{
		// 参照系 (外部ライフタイム依存)
		const CanonicalAxis*		ax = nullptr;
		const CanonicalFSM*			fsm = nullptr;
		const EnvAssemblerPlan*		plan = nullptr;		// 派生条件の実行レシピ
		const PrecomputedEvalPlans* eval = nullptr;		// (state, profile) → NextEvalSet

		// 実行状態
		// チェーン遷移は現状考えない
		std::uint32_t curState = 0;		// 現在state (local)
		std::uint32_t prevState= 0;		// 一つ前のstate (local)

		std::uint32_t curProfile = 0;		// 現在profile (local)
		std::uint32_t prevProfile = 0;		// 一つ前のprofile (local)

		// 条件ビット
		ConditionBank conds;				// Derived/Base/Latched/Override の4ch

		// 1frame cache
		std::span<const std::uint32_t> requiredBits;
		std::span<const std::uint32_t> candidateSlots;

		// 初期化
		void init
		(
			const CanonicalAxis& ax_,
			const CanonicalFSM& fsm_,
			const EnvAssemblerPlan& plan_,
			const PrecomputedEvalPlans& eval_,
			std::uint32_t initStateLocal,
			std::uint32_t initProfileLocal
		)
		{
			ax = &ax_;
			fsm = &fsm_;
			plan = &plan_;
			eval = &eval_;
			curState = initStateLocal;
			prevState = initStateLocal;
			curProfile = initProfileLocal;
			prevProfile = initProfileLocal;

			// 事前にチャンネルのワード数を確保(軸の cond 個数に合わせる)
			const std::uint32_t numConds = (std::uint32_t)ax->condOrder.size();
			for (int i = 0; i < 4; i++)
				conds.ch[i].ensureSize(numConds);
		}

		void refreshPlanForThisFrame()
		{
			const auto set = GetNextEvalSet(*eval, curState, curProfile);
			requiredBits = set.condBits;
			candidateSlots = set.slots;
		}

		// フレーム冒頭の規約 (Latched の寿命管理や一時クリアなど)
		void beginFrame(float dtSeconds)
		{
			conds.beginFrame();
			// Latched 寿命管理
			conds.tickLatchedFrames();
			conds.tickLatchedSeconds(dtSeconds);

			// 今フレームの必要条件を確定
			refreshPlanForThisFrame();
		}

		bool ApplyDecision(const Decision& d);

		// 1フレームの決定
		//「同フレーム複数 tick はしない」
		Decision tick(const IFieldReader& reader);


		void setBase(std::uint32_t condBit, bool v)
		{
			conds.ch[static_cast<std::size_t>(CondChannel::Base)].set(condBit, v);
		}

		void pulseLatched(std::uint32_t condBit)
		{
			conds.ch[static_cast<std::size_t>(CondChannel::Latched)].set(condBit, true);
		}

		void clearLatched()
		{
			conds.ch[static_cast<std::size_t>(CondChannel::Latched)].clearAll();
		}
	};
}// namespace Engine::FSM::Core