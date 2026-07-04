#include "Engine/FSM/Public/Core/Analysis.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"

#include <algorithm>
#include <cassert>

namespace Engine::FSM::Core
{
	// CSR区間長から出次数があるか判定
	static inline bool CellHasEdges(const CanonicalFSM& f, std::uint32_t stateLocal, std::uint32_t slotLocal)
	{
		const std::uint32_t S = f.numSlots;
		const std::uint32_t cell = stateLocal * S + slotLocal;
		return f.ofs[cell] < f.ofs[cell + 1];
	}

	// (state, profile) について 
	// (評価候補となるスロット集合, その候補が要求する条件ビットのユニーク集合, 各スロットに対応する条件ビット)を前計算して返す
	PerStateProfilePlan BuildPlanFor(
		const CanonicalFSM& f,
		std::uint32_t stateLocal,
		std::uint32_t profileLocal)
	{
		PerStateProfilePlan plan;

		const std::uint32_t N = f.numStates;
		const std::uint32_t P = f.numProfiles;
		const std::uint32_t S = f.numSlots;

		// 範囲外なら空を返す.
		assert(stateLocal < N && profileLocal < P);
		if (stateLocal >= N || profileLocal >= P) return plan;

		// 出次数>0の slot を候補に
		plan.candidateSlots.reserve(S);
		for (std::uint32_t s = 0; s < S; ++s)
		{
			if (CellHasEdges(f, stateLocal, s))
			{
				plan.candidateSlots.push_back(s);
			}
		}

		// condBitOfSlot を numSlots長 で初期化
		plan.condBitOfSlot.assign(S, UINT32_MAX);
	
		// 候補slotに対する condOf を拾って valid なものだけをつなげる
		std::vector<std::uint32_t> tmpBits;
		tmpBits.reserve(plan.candidateSlots.size());

		for (std::uint32_t s : plan.candidateSlots)
		{
			const CondID cid = f.condOf[profileLocal * S + s];
			if (cid.valid())
			{
				plan.condBitOfSlot[s] = cid.v;
				tmpBits.push_back(cid.v);
			}
		}

		std::sort(tmpBits.begin(), tmpBits.end());
		tmpBits.erase(std::unique(tmpBits.begin(), tmpBits.end()), tmpBits.end());
		plan.condBits = std::move(tmpBits);


		// 先頭edgeから "優先度キー" を作って candidateSlots を並べ替え
		auto slot_key = [&](uint32_t s)->uint64_t {
			const uint32_t cell = stateLocal * S + s;
			const uint32_t a = f.ofs[cell], b = f.ofs[cell + 1];
			if (a == b) return ~uint64_t(0); // 出次数0 (来ないはずだが保険)
			const auto& e = f.edges[a];      // 先頭 = prio最大, toIdx最小
			// Decide の pack_key と整合する順序（小さいほど優先）
			return (uint64_t(255 - e.prio) << 56) | (uint64_t(e.toIdx) << 24) | uint64_t(s);
			};

		if (plan.candidateSlots.size() > 1)
		{
			std::sort(plan.candidateSlots.begin(), plan.candidateSlots.end(),
				[&](uint32_t lhs, uint32_t rhs) {
					return slot_key(lhs) < slot_key(rhs);
				});
		}

		return plan;
	}

	// TODO: メモリ量の懸念
	// table[N][S] について、BuildPlanForを回し、前計算して計画を table[st][pr]に格納
	PrecomputedEvalPlans BuildEvalPlans(const CanonicalFSM& f)
	{
		PrecomputedEvalPlans ps;
		const std::uint32_t N = f.numStates;
		const std::uint32_t P = f.numProfiles;

		ps.table.resize(N);
		for (std::uint32_t st = 0; st < N; ++st)
		{
			ps.table[st].resize(P);
			for(std::uint32_t pr = 0; pr < P; ++pr)
			{
				ps.table[st][pr] = BuildPlanFor(f, st, pr);
			}
		}

		return ps;
	}

	// 前計算から(次に評価すべき候補スロット集合, その評価に必要な条件ビットのユニーク集合)をコピーなしで返す
	NextEvalSet GetNextEvalSet(
		const PrecomputedEvalPlans& ps,
		std::uint32_t stateLocal,
		std::uint32_t profileLocal
	)
	{

		// 
		assert(stateLocal < ps.table.size());
		if (stateLocal >= ps.table.size())
		{
			return NextEvalSet{ {}, {} };
		}

		assert(profileLocal < ps.table[stateLocal].size());
		if (profileLocal >= ps.table[stateLocal].size())
		{
			return NextEvalSet{ {} , {} };
		}

		const auto& pl = ps.table[stateLocal][profileLocal];
		return NextEvalSet{
			std::span<const std::uint32_t>(pl.candidateSlots.data(), pl.candidateSlots.size()),
			std::span<const std::uint32_t>(pl.condBits.data(),		pl.condBits.size())
		};
	}
}// namespace Engine::FSM::Core