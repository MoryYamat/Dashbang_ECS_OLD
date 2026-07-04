#pragma once


#include "Engine/FSM/Public/Core/Analysis.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <string_view>

#include <span>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <functional>

namespace Engine::FSM::Core
{

	
	enum class CondKind : std::uint8_t
	{
		Bit = 0,
		CompareF = 1,
		InRange = 2,
	};

	// kind == Bit: なし
	// kind == CompareF: GT/GE/LT/LE
	// kind == InRange:  InRange
	enum class CmpOp : std::uint8_t
	{
		GT = 0,
		GE = 1,
		LT = 2,
		LE = 3,
		InRange = 4
	};

	struct DerivedCondSpec
	{
		CondKind kind{};
		std::uint16_t field{};
		CmpOp op{};
		std::uint16_t u16_0{};
		float f32_0{};
		float f32_1{};
		std::uint32_t outBitIndex{};
		std::string fieldName;			// AxisDTO.condDefs.field を保持
	};

	struct EnvAssemblerPlan
	{
		struct Op
		{
			CondKind kind{};
			std::uint16_t field{};
			CmpOp op{};
			std::uint16_t u16_0{};
			float f32_0{};
			float f32_1{};
			std::uint32_t outBitIndex{};
		};
		std::vector<Op> ops;
	};

	struct TransitionEdge 
	{ 
		std::uint32_t toIdx;
		uint8_t prio; 
	};

	struct CanonicalFSM
	{
		FSMID id{};
		std::string name;
		std::uint32_t numStates = 0;
		std::uint32_t numSlots = 0;
		std::uint32_t numProfiles = 0;

		// i番目の(state,slot)のedges開始位置 (ofs[i+1]-ofs[i]:出次数)
		std::vector<std::uint32_t> ofs;			// ofs[i] idx = (from,slot) の組を表す: size = numStates * numSlots + 1 
		std::vector<TransitionEdge> edges;	// ソート後の (to, prio)の組

		std::vector<CondID> condOf;			// size = numProfiles * numSlots

		// ローカル→グローバルの写像
		std::vector<StateID> local2GlobalState;
		std::vector<SlotID> local2GlobalSlot;
		std::vector<ProfileID> local2GlobalProfile;

		// 常に真のCondID
		std::uint32_t alwaysTrueBit = UINT32_MAX;

		std::uint16_t version = 1;
	};

	struct CanonicalAxis
	{
		AxisID axis;
		std::string axisName;

		// 軸宇宙（辞書順採番に使った最終順序）
		std::vector<std::string> stateOrder;
		std::vector<std::string> condOrder;
		std::vector<std::string> slotOrder;
		std::vector<std::string> profileOrder;

		std::unordered_map<std::string, std::uint32_t> stateIdxByName;
		std::unordered_map<std::string, std::uint32_t> condIdxByName;
		std::unordered_map<std::string, std::uint32_t> slotIdxByName;
		std::unordered_map<std::string, std::uint32_t> profileIdxByName;

		std::vector<CanonicalFSM> fsms;
		std::unordered_map<std::string, std::uint32_t> fsmIdxByName;

		std::vector<DerivedCondSpec> derivedSpecs;

	};

	struct FSMCatalog
	{    
		std::vector<CanonicalAxis> axes;
		// std::unordered_map<std::string, AxisID> axisByName;		// TODO: ビルド時に埋める
	};


	// Cond
	struct EvalCtx
	{
		std::uint32_t entity;
	};

	struct EnvSnapshot
	{
		// 事前にまとめて計算した値．移動入力、時刻など
		virtual ~EnvSnapshot() = default;

		virtual bool testCondBit(std::uint32_t i) const { return false; }
	};

	struct BitEnvSnapshot : EnvSnapshot
	{
		using Word = std::uint32_t;
		std::vector<Word> bits;// 真偽結果

		static constexpr std::uint32_t kWordShift = 5;	//32bit
		static constexpr std::uint32_t kWordMask = 31;	

		void ensureSize(std::uint32_t numConds)
		{
			const auto words = (numConds + kWordMask) >> kWordShift;
			if (bits.size() < words) bits.resize(words, Word{ 0 });
		}

		void clearAll()
		{
			std::fill(bits.begin(), bits.end(), Word{ 0 });
		}

		// v(true/false)をbitsに反映→ true:1 / false: 0
		void set(std::uint32_t id, bool v)
		{
			const auto w = id >> kWordShift;		// wordブロック検出
			const auto b = id & kWordMask;		// bit番号計算

			if (w >= bits.size()) bits.resize(w + 1, Word{ 0 });
			const Word m = Word{ 1 } << b;
			if (v) bits[w] |= m;
			else bits[w] &= ~m;
		}

		// 
		bool testCondBit(std::uint32_t id) const override
		{
			const auto w = id >> kWordShift;
			const auto b = id & kWordMask;
			if (w >= bits.size()) return false;
			const Word m = Word{ 1 } << b;
			return (bits[w] & m) != 0;
		}
	};

	struct AxisRuntime
	{
		AxisID id;
		const CanonicalAxis* canon = nullptr;// 読み取り用

		EnvAssemblerPlan plan;

		std::vector<PrecomputedEvalPlans> evals;
	};

	struct Decision
	{
		std::uint32_t from;	// local state idx
		std::uint32_t to;	// local state idx (unchanged => from)
		bool changed;
		std::uint8_t prio;
		std::uint32_t slot;
	};


	struct AxisRuntimeDB
	{
		std::unordered_map<std::string, AxisRuntime> axes;
		AxisRuntime* get(const std::string& name)
		{
			auto it = axes.find(name);
			return it != axes.end() ? &it->second : nullptr;
		}

		AxisRuntime& ensure(const CanonicalAxis& ax)
		{
			auto [it, ok] = axes.emplace(ax.axisName, AxisRuntime{});
			it->second.id = ax.axis;
			it->second.canon = &ax;
			return it->second;
		}


	};




	// ID→値
	// フィールド読み出しインターフェース(ゲーム側スナップショット)
	struct IFieldReader
	{
		virtual ~IFieldReader() = default;
		virtual float getF32(std::uint16_t fieldIndex) const = 0;
	};

	// 部分評価: requiredBitsに含まれるoutBitIndexだけ実行
	void ExecuteDerivedCondsFiltered
	(
		const EnvAssemblerPlan& plan,
		std::span<const std::uint32_t> requiredBits,
		const IFieldReader& reader,
		BitEnvSnapshot& outBits
	);

	//struct NamedCondBinding
	//{
	//	std::string_view name;
	//	// CondTable::Fn fn;
	//};
	
	//struct CondTable
	//{
	//	using Fn = bool(*)(const EnvSnapshot&, const EvalCtx&);
	//	std::vector<Fn> fns;
	
	//	void init(std::size_t numConds)
	//	{
	//		fns.assign(numConds, nullptr);
	//	}
	
	//	void bind(CondID id, Fn fn)
	//	{
	//		if (!id.valid()) return;
	//		if (id.v >= fns.size()) fns.resize(id.v + 1, nullptr);
	//		fns[id.v] = fn;
	//	}
	
	//	bool eval(CondID c, const EnvSnapshot& env, const EvalCtx& ctx) const
	//	{
	//		const auto i = c.v;
	//		if (!c.valid() || i >= fns.size()) return false;
	//		Fn fn = fns[i];
	//		return fn ? fn(env, ctx) : false;
	//	}
	//};
}