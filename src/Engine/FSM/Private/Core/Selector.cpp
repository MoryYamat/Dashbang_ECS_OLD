#include "Engine/FSM/Public/Core/Types.hpp"


#include <span>

namespace Engine::FSM::Core
{
	//(from, slot, profile) の組から条件を評価してどのノードへ進むかを決定する決定器関数
	Decision DecideNext_BySingleSlot
	(
		const CanonicalFSM& f,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::uint32_t slotLocal,
		const EnvSnapshot& env
	)
	{
		Decision d{ fromLocal, fromLocal, false, 0u, slotLocal };

		const std::uint32_t N = f.numStates;
		const std::uint32_t P = f.numProfiles;
		const std::uint32_t S = f.numSlots;

		if (fromLocal >= N || profileLocal >= P || slotLocal >= S) return d;

		// (prof, slot) -> CondID
		const CondID cid = f.condOf[profileLocal * S + slotLocal];
		if (!cid.valid()) return d;
		if (cid.v != f.alwaysTrueBit && !env.testCondBit(cid.v)) return d;

		// CSR
		const std::uint32_t cell = fromLocal * S + slotLocal;
		const std::uint32_t a = f.ofs[cell];
		const std::uint32_t b = f.ofs[cell + 1];
		if (a == b) return d;

		//
		const auto& e = f.edges[a];
		d.to = e.toIdx;
		d.prio = e.prio;
		d.changed = (d.to != d.from);
		return d;
	}

	Decision DecideNext_Slots
	(
		const CanonicalFSM& f,
		// const AxisRuntime& ax,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::span<const std::uint32_t> slots,
		const EnvSnapshot& env//,
		// const EvalCtx& ctx
	)
	{
		Decision out{ fromLocal, fromLocal, false };
		const std::uint32_t N = f.numStates;
		const std::uint32_t P = f.numProfiles;
		const std::uint32_t S = f.numSlots;
		if (fromLocal >= N || profileLocal >= P) return out;

		// TODO: オーバーフロー対策
		// 複合順序を1回の比較に畳み込むキー(小さいほど良い)
		// （“prio降順 → to昇順 → slot昇順”）
		auto pack_key = [](std::uint8_t prio, std::uint32_t toIdx, std::uint32_t slot)->std::uint64_t
			{
				return (std::uint64_t(255 - prio) << 56)
					| (std::uint64_t(toIdx) << 24)
					| std::uint64_t(slot);
			};

		bool have = false;
		std::uint64_t bestKey = ~std::uint64_t(0);
		Decision best{ fromLocal, fromLocal, false, 0u, UINT32_MAX };

		for (uint32_t s : slots) {
			if (s >= S) continue;

			// 単一セルの意思決定 (条件評価+セル内の最優先 to を確定)
			Decision d = DecideNext_BySingleSlot(f, fromLocal, profileLocal, s, env);
			if (!d.changed) continue;



			const std::uint64_t key = pack_key(d.prio, d.to, d.slot);
			if (!have || key < bestKey) {
				have = true; 
				bestKey = key; 
				best = d;
			}
		}

		if (have) out = best;
		return out;
	}
}


// ①	Decide層の責務	DecideはFSMの「入力＝CondBit群」と「構造（ofs / edges）」を結合して結果を返す。条件評価は誰の責務か？環境？FSM？
// ②	bit - onlyモードの意味	Env側がCondBitを提供する時、FSMは何を「仮定」して動くか？ CondIDの整合性や未設定状態をどう扱うか？
// ③	Profile×Slotの行列構造	condOf[profile][slot] の持つ意味を、どこで固定するか？ runtime生成か、build時決定か？
// ④	複数Slot間の優先解決	prioをどう扱う？ CSR上の順序？ DecideNext_Slotsがどの範囲を探索する？
// ⑤	拡張設計（非bit, Paramあり）	今はbit - onlyだが、将来的に評価関数つきCondをどう統合するか？