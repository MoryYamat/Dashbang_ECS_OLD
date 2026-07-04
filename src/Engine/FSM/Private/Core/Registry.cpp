#include "Engine/FSM/Public/Core/Registry.hpp"

#include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/DTO.hpp"

#include <cstddef>
#include <string>
#include <cstdint>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <queue>

namespace Engine::FSM::Core
{
	void FSMRegistry::add(AxisDTO axis)
	{
		axes_.emplace_back(std::move(axis));
	}

	void FSMRegistry::add(FSMDTO fsm)
	{
		fsms_.emplace_back(std::move(fsm));
	}

	// 文字列→CondKind
	static bool parseCondKind(const std::string& s, CondKind& out)
	{
		if (s == "Bit") { out = CondKind::Bit;				return true; }
		if (s == "CompareF") { out = CondKind::CompareF;	return true; }
		if (s == "InRange") { out = CondKind::InRange;		return true; }
		return false;
	}

	// 文字列→CmpOp
	static bool parseCmpOp(const std::string& s, CmpOp& out)
	{
		if (s == ">") { out = CmpOp::GT;		return true; }
		if (s == ">=") { out = CmpOp::GE;		return true; }
		if (s == "<") { out = CmpOp::LT;		return true; }
		if (s == "<=") { out = CmpOp::LE;		return true; }
		if (s == "in") { out = CmpOp::InRange;	return true; }
		return false;
	}


	static bool parseFloat(const std::string& s, float& out)
	{
		if (s.empty())
			return false;
		char* end = nullptr;
		const char* str = s.c_str();
		// float に変換
		out = std::strtof(str, &end);// TODO: ロケール考慮する必要あり

		// パースができていて、かつ全体を消費しているか？
		if (end == str || *end != '\0')
		{
			return false;
		}

		return true;
	}

	// AxisDTO の condDefs を検証し、CanonicalAxis.derivedSpecsを作る
	static void buildDerivedSpecsFromAxis
	(
		const AxisDTO& a,			// 元のAxisDTO
		const AxisTable& t,			// 辞書化済み(condByName)
		BuildErrors& err,
		std::vector<DerivedCondSpec>& outSpecs
	)
	{
		std::unordered_set<std::string> seen;
		seen.reserve(a.condDefs.size());

		for (const auto& d : a.condDefs)
		{
			const std::string owner = "Axis '" + a.axis + "' condDef '" + d.cond + "'";
			if (d.cond.empty())
			{
				err.err(owner + " has empty cond name");
				continue;
			}
			if (!seen.insert(d.cond).second)
			{
				err.err(owner + " duplicate condDef");
				continue;
			}
			// cond が condU に存在
			auto itCond = t.condByName.find(d.cond);
			if (itCond == t.condByName.end())
			{
				err.err(owner + "refers to unknown cond in condU");
				continue;
			}

			CondKind kind{};
			if (!parseCondKind(d.kind, kind))
			{
				err.err(owner + " has invalid kind '" + d.kind + "'");
				continue;
			}

			CmpOp op{};
			if (kind == CondKind::Bit)
			{
				// op/field/th 等は未使用。何もせず許可
			}
			else if (kind == CondKind::CompareF)
			{
				if (!parseCmpOp(d.op, op) || (op != CmpOp::GT && op != CmpOp::GE && op != CmpOp::LT && op != CmpOp::LE))
				{
					err.err(owner + "invalid op for CompareF: '" + d.op + "'");
					continue;
				}
				if (d.field.empty())
				{
					err.err(owner + " CompareF requires filed name");
					continue;
				}
				float th{};
				if (!parseFloat(d.th, th))
				{
					err.err(owner + " CompareF requires numeric 'th'");
					continue;
				}
				DerivedCondSpec s{};
				s.kind = kind;
				s.op = op;
				s.f32_0 = th;
				s.f32_1 = 0.0f;
				s.field = 0;
				s.fieldName = d.field;
				s.u16_0 = 0;
				s.outBitIndex = itCond->second.v;
				outSpecs.push_back(std::move(s));
				continue;
			}
			else if (kind == CondKind::InRange)
			{
				if (!parseCmpOp(d.op, op) || op != CmpOp::InRange)
				{
					err.err(owner + " InRange requires op 'in'");
					continue;
				}
				if (d.field.empty())
				{
					err.err(owner + " InRange requires field name");
					continue;
				}
				float lo{}, hi{};
				if (!parseFloat(d.low, lo) || !parseFloat(d.high, hi))
				{
					err.err(owner + " InRange requires numeric 'low' and 'high'");
					continue;
				}
				if (lo > hi)
				{
					err.err(owner + " InRange low must be <= high");
					continue;
				}
				DerivedCondSpec s{};
				s.kind = kind;
				s.op = op;
				s.f32_0 = lo;
				s.f32_1 = hi;
				s.field = 0;
				s.fieldName = d.field;
				s.u16_0 = 0;
				s.outBitIndex = itCond->second.v;
				outSpecs.push_back(std::move(s));
				continue;
			}

			// kind == Bit の時
			DerivedCondSpec s{};
			s.kind = kind;
			s.op = CmpOp::GT;// 未使用
			s.f32_0 = s.f32_1 = 0.0f;
			s.field = 0;
			s.fieldName.clear();
			s.u16_0 = 0;
			s.outBitIndex = itCond->second.v;
			outSpecs.push_back(std::move(s));
		}

		// 追加の安全策：condU にあるのに condDefs が無い項目をどう扱うか（v1はエラー推奨）
		// 必須にするなら以下を有効化
		/*
		for (const auto& name : t.condOrder) {
			if (!seen.count(name)) {
				err.err("Axis '" + a.axis + "': cond '" + name + "' is in condU but has no condDef");
			}
		}
		*/
	}

	std::unordered_map<std::string, std::uint32_t>
		FSMRegistry::indexAxes(const std::vector<AxisDTO>& axes, BuildErrors& err)
	{
		std::unordered_map<std::string, std::uint32_t> map;
		map.reserve(axes.size());
		for (std::uint32_t i = 0; i < axes.size(); ++i)
		{
			const auto& name = axes[i].axis;
			if (name.empty()) {
				err.err("Axis name is empty at index " + std::to_string(i));
				continue;
			}
			if (!map.emplace(name, i).second) {
				err.err("Duplicate Axis name: " + name);
			}
		}
		return map;
	}

	BuildResult FSMRegistry::build(BuildStrictness policy) const
	{
		BuildErrors err;

		//const auto axisIndex = indexAxes(axes_, err);

		std::printf("FSMRegistry::build: axes_.size=%zu fsms_.size=%zu\n",
			axes_.size(), fsms_.size());


		// 辞書順で Axis の並びを決定 → 先頭から0..N-1を採番
		std::vector<std::uint32_t> order;
		order.reserve(axes_.size());
		for (std::uint32_t i = 0; i < axes_.size(); ++i)
			order.push_back(i);

		std::stable_sort(order.begin(), order.end(),
			[&](std::uint32_t a, std::uint32_t b)
			{
				return axes_[a].axis < axes_[b].axis;
			});

		// 
		std::vector<AxisTable> tables;
		tables.reserve(order.size());
		for (std::uint32_t rank = 0; rank < order.size(); ++rank)
		{
			const std::uint32_t i = order[rank];
			AxisTable t = makeAxisTables(axes_[i], err);
			t.id = AxisID{ rank };
			tables.emplace_back(std::move(t));
		}

		// Catalog skeleton 
		FSMCatalog cat = assembleCatalogSkeleton(tables);

		// Derived を カタログへ
		for (std::uint32_t rank = 0; rank < order.size(); ++rank)
		{
			const std::uint32_t i = order[rank];
			auto& ax = cat.axes[rank];
			buildDerivedSpecsFromAxis(axes_[i], tables[rank], err, ax.derivedSpecs);
		}

		// build() の Derived 構築直後などに（デバッグ専用）
		for (std::uint32_t rank = 0; rank < cat.axes.size(); ++rank) {
			const auto& ax = cat.axes[rank];
			std::printf("[Derived] Axis '%s' specs=%zu\n",
				ax.axisName.c_str(), ax.derivedSpecs.size());
			for (const auto& s : ax.derivedSpecs) {
				const char* k =
					(s.kind == CondKind::Bit) ? "Bit" :
					(s.kind == CondKind::CompareF) ? "CompareF" : "InRange";
				const char* o =
					(s.op == CmpOp::GT) ? ">" :
					(s.op == CmpOp::GE) ? ">=" :
					(s.op == CmpOp::LT) ? "<" :
					(s.op == CmpOp::LE) ? "<=" : "in";
				std::printf("  - kind=%s op=%s field='%s' th=[%f,%f] outBit=%u\n",
					k, o, s.fieldName.c_str(), s.f32_0, s.f32_1, s.outBitIndex);
			}
		}


		// 軸名→rank
		std::unordered_map<std::string, std::uint32_t> axisNameToRank;
		for (std::uint32_t r = 0; r < order.size(); ++r)
		{
			axisNameToRank.emplace(axes_[order[r]].axis, r);
		}

		// 各FSMを検証→Canonical化→push
		for (const auto& fsm : fsms_)
		{
			FSMCheck chk;
			if (!validateFSM(fsm, axisNameToRank, tables, err, chk)) continue;

			const AxisTable& at = tables[chk.axisRank];
			CanonicalFSM cfsm = makeCanonicalFSM(fsm, at, chk, err);

			auto& ax = cat.axes[chk.axisRank];

			// FSMIdx
			const std::uint32_t newIndex = static_cast<std::uint32_t>(ax.fsms.size());
			if (!ax.fsmIdxByName.emplace(fsm.fsm, newIndex).second)
			{
				err.err("Axis '" + ax.axisName + "': duplicate FSM name '" + fsm.fsm + "'");

				continue;
			}

			// 追加
			ax.fsms.emplace_back(std::move(cfsm));
		}

		if (policy == BuildStrictness::Strict && !err.ok())
		{
			cat.axes.clear();
		}

		return BuildResult{ std::move(cat), std::move(err) };
	}

	AxisTable FSMRegistry::makeAxisTables(const AxisDTO& a, BuildErrors& err)
	{
		AxisTable t{};
		t.axisName = a.axis;
		t.version = a.version;

		// 共通ヘルパ: ユニバース配列を検査→辞書順ユニーク→name→ID と order を埋める
		auto build_domain = [&](const char* kind,	// DTOの中身の種類
			const std::vector<std::string>& names,	// DTOの中身
			auto& outMap,							// 
			std::vector<std::string>& outOrder)
			{
				outMap.clear();
				outOrder.clear();
				outMap.reserve(names.size());
				outOrder.reserve(names.size());
				

				// 空名/重複チェック
				std::unordered_set<std::string> seen;
				seen.reserve(names.size());
				for (std::uint32_t i = 0; i < names.size(); ++i)
				{
					const auto& n = names[i];
					if (n.empty())
					{
						// error
						err.err(std::string("Axis '") + a.axis + "': " + kind +
							" contains empty name at index " + std::to_string(i));

						continue;
					}
					if (!seen.insert(n).second)// 追加 & 重複チェック
					{
						err.err(std::string("Axis '") + a.axis + "': " + kind +
							" duplicate name '" + n + "'");
					}
				}

				// ユニーク集合を辞書順に並べ替え
				std::vector<std::string> uniq;
				uniq.assign(seen.begin(), seen.end());
				std::sort(uniq.begin(), uniq.end());

				// 0..N-1 を割り当て (辞書順採番) し、mapとorder を埋める
				for (std::uint32_t i = 0; i < uniq.size(); ++i)
				{
					const std::string& n = uniq[i];

					//　StrongID は各 outMap の value型から推論
					using MapT = std::remove_reference_t<decltype(outMap)>;
					using IdT = typename MapT::mapped_type;
					outMap.emplace(n, IdT{ i });
					outOrder.push_back(n);
				}
			};

		build_domain("stateU", a.stateU, t.stateByName/*(string, ID)*/, t.stateOrder);
		build_domain("condU", a.condU, t.condByName, t.condOrder);
		build_domain("slotU", a.slotU, t.slotByName, t.slotOrder);
		build_domain("profileU", a.profileU, t.profileByName, t.profileOrder);

		return t;
	}

	FSMCatalog FSMRegistry::assembleCatalogSkeleton(const std::vector<AxisTable>& tables)
	{
		FSMCatalog cat;
		cat.axes.reserve(tables.size());

		for (const auto& t : tables)
		{
			CanonicalAxis ax{};
			ax.axisName = t.axisName;
			ax.axis = t.id;			// AxisID をそのまま反映

			// Axis 全域の辞書順オーダを丸ごとコピー
			ax.stateOrder = t.stateOrder;
			ax.condOrder = t.condOrder;
			ax.slotOrder = t.slotOrder;
			ax.profileOrder = t.profileOrder;

			for (std::uint32_t i = 0; i < ax.stateOrder.size(); ++i){ ax.stateIdxByName.emplace(ax.stateOrder[i], i); }
			for (std::uint32_t i = 0; i < ax.condOrder.size(); ++i) { ax.condIdxByName.emplace(ax.condOrder[i], i); }
			for (std::uint32_t i = 0; i < ax.slotOrder.size(); ++i) { ax.slotIdxByName.emplace(ax.slotOrder[i], i); }
			for (std::uint32_t i = 0; i < ax.profileOrder.size(); ++i) { ax.profileIdxByName.emplace(ax.profileOrder[i], i); }

			// ax.fsms はこの段階で空 (後でFSMを詰める)
			cat.axes.emplace_back(std::move(ax));
		}

		return cat;
	}

	static bool uniqNames(const std::vector<std::string>& names,
		const char* what, const std::string& owner, BuildErrors& err)
	{
		std::unordered_set<std::string> s;
		bool ok = true;
		for (size_t i = 0; i < names.size(); ++i)
		{
			if (names[i].empty())
			{
				err.err(owner + ": " + what + " contains empty name at index " + std::to_string(i));
				ok = false;
				continue;
			}
			if (!s.insert(names[i]).second)
			{
				err.err(owner + ": " + what + " duplicate name '" + names[i] + "'");
				ok = false;
			}
		}

		return ok;
	}

	static bool checkProfileDefs
	(
		const FSMDTO& fsm,
		const AxisTable& at,
		BuildErrors& err,
		ExtendsCheck& out
	)
	{
		bool ok = true;
		const std::string owner = "FSM '" + fsm.fsm + "' (axis '" + fsm.axis + "')";

		// name -> idx
		std::unordered_map<std::string, std::uint32_t> name2idx;
		name2idx.reserve(fsm.profile_defs.size());
		for (std::uint32_t i = 0; i < fsm.profile_defs.size(); ++i)
		{
			const auto& d = fsm.profile_defs[i];
			if (d.name.empty())
			{
				err.err(owner + ": profile_defs has empty name");
				ok = false;
				continue;
			}
			if (!name2idx.emplace(d.name, i).second)
			{
				err.err(owner + ": duplicate profile_def '" + d.name + "'");
				ok = false;
			}
		}

		// 定義確認
		for (auto& pn : fsm.profiles)
		{
			if (name2idx.find(pn) == name2idx.end())
			{
				err.err(owner + ": profile '" + pn + "' has no definition in profile_defs");
				ok = false;
			}
		}

		// 参照存在 & 自己参照禁止、3) DAG チェック（Kahn）
		const std::uint32_t N = (std::uint32_t)fsm.profile_defs.size();
		std::vector<std::uint32_t> indeg(N, 0);
		std::vector<std::vector<std::uint32_t>> adj(N);

		for (std::uint32_t i = 0; i < N; ++i)
		{
			const auto& d = fsm.profile_defs[i];
			if (d.extends.empty()) continue;// 親なし
			auto it = name2idx.find(d.extends);
			if (it == name2idx.end())// 存在しない親
			{
				err.err(owner + ": profile_def '" + d.name + "' extends unknown '" + d.extends + "'");
				ok = false;
				continue;
			}
			std::uint32_t p = it->second;// 親ノードのindex
			if (p == i)//自己参照
			{
				err.err(owner + ": profile_def '" + d.name + "' extends itself");
				ok = false;
				continue;
			}
			adj[p].push_back(i);
			indeg[i]++;				// 次数カウント
		}

		// kahn
		std::queue<std::uint32_t> q;
		for (std::uint32_t i = 0; i < N; ++i)
		{
			if (indeg[i] == 0) q.push(i);// 親ノードをキューへ
		}
		out.topo.clear();
		out.topo.reserve(N);
		while (!q.empty())
		{
			auto u = q.front();
			q.pop();
			out.topo.push_back(u);				// 親→子の順に並ぶ
			for (auto v : adj[u])				// 「uへの依存」を1つ外す
				if (--indeg[v] == 0) q.push(v);	// もう待ち親がなければ次に処理可能へ
		}
		if (out.topo.size() != N)
		{
			err.err(owner + ": profile_defs has cyclic extends");
			ok = false;
		}

		//binds の slot/cond が Axisに存在
		for (auto& d : fsm.profile_defs)
		{
			for (auto& b : d.binds)
			{
				if (at.slotByName.find(b.slot) == at.slotByName.end())
				{
					err.err(owner + ": profiledef '" + d.name + "' uses unknown slot '" + b.slot + "'");
					ok = false;
				}
				if (at.condByName.find(b.cond) == at.condByName.end())
				{
					err.err(owner + ": profile_def '" + d.name + "' uses unknown cond '" + b.cond + "'");
					ok = false;
				}
			}
		}
		return ok;
	}

	// FSMDTO を検証
	bool FSMRegistry::validateFSM
	(
		const FSMDTO& fsm,
		const std::unordered_map<std::string, std::uint32_t>& axisNameToRank,
		const std::vector<AxisTable>& tables,
		BuildErrors& err,
		FSMCheck& out
	)
	{
		bool ok = true;
		const std::string owner = "FSM '" + fsm.fsm + "' (axis '" + fsm.axis + "')";

		// 
		auto ait = axisNameToRank.find(fsm.axis);
		if (ait == axisNameToRank.end())
		{
			err.err(owner + ": axis not found");
			return false;
		}
		out.axisRank = ait->second;
		const AxisTable& at = tables[out.axisRank];

		// states/profiles のチェック
		ok &= uniqNames(fsm.states, "states", owner, err);
		ok &= uniqNames(fsm.profiles, "profiles", owner, err);

		// states -> Axis stateU に存在するかチェック
		out.local2GlobalState.resize(fsm.states.size());
		for (size_t i = 0; i < fsm.states.size(); ++i)
		{
			auto it = at.stateByName.find(fsm.states[i]);
			if (it == at.stateByName.end())
			{
				err.err(owner + ": unknown state '" + fsm.states[i] + "'");
				ok = false;
				continue;
			}

			out.local2GlobalState[i] = it->second;
		}

		// profiles -> Axis profileU 
		out.local2GlobalProfile.resize(fsm.profiles.size());
		for (size_t i = 0; i < fsm.profiles.size(); ++i)
		{
			auto it = at.profileByName.find(fsm.profiles[i]);
			if (it == at.profileByName.end())
			{
				err.err(owner + ": unknown profile '" + fsm.profiles[i] + "'");
				ok = false;
				continue;
			}
			out.local2GlobalProfile[i] = it->second;
		}

		// profile_defs の 重複名
		{
			std::unordered_set<std::string> seen;
			for (auto& d : fsm.profile_defs)
			{
				if (d.name.empty())
				{
					err.err(owner + ": profile_defs has empty name");
					ok = false;
					continue;
				}
				if (!seen.insert(d.name).second)
				{
					err.err(owner + ": duplicate profile_defs '" + d.name + "'");
					ok = false;
				}
			}
		}

		// transitions の from/ to/ slot 検証
		{
			std::unordered_set<std::string> stateSet(fsm.states.begin(), fsm.states.end());
			for (auto& tr : fsm.transitions)
			{
				if (!stateSet.count(tr.from))
				{
					err.err(owner + ": transition.from unknown '" + tr.from + "'");
					ok = false;
				}
				if (!stateSet.count(tr.to))
				{
					err.err(owner + ": transition.to unknown '" + tr.to + "'");
					ok = false;
				}
				if (at.slotByName.find(tr.slot) == at.slotByName.end())
				{
					err.err(owner + ": transition.slot unknown '" + tr.slot + "'");
					ok = false;
				}
			}
		}
		ExtendsCheck ec;
		ok &= checkProfileDefs(fsm, at, err, ec);

		return ok;
	}


	void FSMRegistry::buildCondOf
	(
		const FSMDTO& fsm,
		const AxisTable& at,
		const ExtendsCheck& ec,
		const std::vector<std::string>& profileOrder,
		std::vector<CondID>& outCondOf
	)
	{
		const std::uint32_t S = (std::uint32_t)at.slotOrder.size();
		const std::uint32_t P = (std::uint32_t)profileOrder.size();
		outCondOf.assign(P * S, CondID{ UINT32_MAX });// 未設定はinvalid

		// name → idx (defs用)
		std::unordered_map<std::string, std::uint32_t> defIndex;
		defIndex.reserve(fsm.profile_defs.size());
		for (std::uint32_t i = 0; i < fsm.profile_defs.size(); ++i)
		{
			defIndex.emplace(fsm.profile_defs[i].name, i);
		}

		// defs にスロット配列を持たせてから topo 順 で解決
		std::vector<std::vector<CondID>> defSlots(fsm.profile_defs.size(), std::vector<CondID>(S, CondID{ UINT32_MAX }));

		// まず binds をその定義自身の配列に適用
		for (std::uint32_t i = 0; i < fsm.profile_defs.size(); ++i)
		{
			const auto& d = fsm.profile_defs[i];
			auto& arr = defSlots[i];
			for (auto& b : d.binds)
			{
				const uint32_t s = at.slotByName.at(b.slot).v;	// slot → slotID(idx)
				arr[s] = at.condByName.at(b.cond);				// // profiles[0] : [CondID, CondID,...],
			}
		}

		// 継承解決
		// 親→子でコピー上書き(topo は親が先)
		for (auto u : ec.topo)// topoの中身はpro_defのidx
		{
			const auto& d = fsm.profile_defs[u];
			if (!d.extends.empty())
			{
				std::uint32_t p = defIndex.at(d.extends);// 親のid
				for (std::uint32_t s = 0; s < S; ++s)
				{
					// 子がbindしている値は無視
					if (!defSlots[u][s].valid()) defSlots[u][s] = defSlots[p][s];// 親の値で補完(無効値も引き継ぐ)
				}
			}
		}

		// fsm.profilesの順で　condOf に反映 (defs にあるが profiles にないものは無視)
		for (std::uint32_t p = 0; p < P; ++p)
		{
			auto it = defIndex.find(profileOrder[p]);
			if (it == defIndex.end()) continue;
			const auto& src = defSlots[it->second];
			for (std::uint32_t s = 0; s < S; ++s)
			{
				outCondOf[p * S + s] = src[s];// ソートしたものを代入しcondOfを得る
			}
		}
	}

	void FSMRegistry::buildCSR
	(
		const FSMDTO& fsm,
		const AxisTable& at,
		const std::unordered_map<std::string, std::uint32_t>& stateLocalIndex,// fsm.state名 → ローカルindex
		std::vector<std::uint32_t>& ofs,
		std::vector<TransitionEdge>& edges
	)
	{
		const std::uint32_t N = (std::uint32_t)fsm.states.size();
		const std::uint32_t S = (std::uint32_t)at.slotOrder.size();

		// セルごとの一時バケツ
		std::vector<std::vector<TransitionEdge>> buckets(N * S);

		// DTO遷移をローカルindexへ写像して投入
		for (const auto& tr : fsm.transitions)
		{
			auto itFrom = stateLocalIndex.find(tr.from);
			auto itTo = stateLocalIndex.find(tr.to);
			auto itSlot = at.slotByName.find(tr.slot);
			if (itFrom == stateLocalIndex.end() ||
				itTo == stateLocalIndex.end() ||
				itSlot == at.slotByName.end())
			{
				// validate済みだが念のため
				continue;
			}
			const std::uint32_t from = itFrom->second;
			const std::uint32_t to = itTo->second;
			const std::uint32_t s = itSlot->second.v;

			const std::uint32_t cell = from * S + s;				// ※※※※ (from,slot)の平坦化 ※※※※
			buckets[cell].push_back(TransitionEdge{ to, tr.prio });	// [cell_i[(to_j,prio_k)](遷移元(idx)(遷移先,優先度))
		}

		// セル内のソート
		for (auto& v : buckets)
		{
			// (prio降順, to 昇順)
			std::stable_sort(v.begin(), v.end(),
				[](const TransitionEdge& a, const TransitionEdge& b)
				{
					if (a.prio != b.prio) return a.prio > b.prio;
					return a.toIdx < b.toIdx;
				});
		}

		// ofs と edges へフラット化
		ofs.assign(N * S + 1, 0);
		std::uint32_t total = 0;
		for (std::uint32_t i = 0; i < N * S; ++i)
		{
			ofs[i] = total;// row_ptr
			total += (std::uint32_t)buckets[i].size();
		}
		ofs[N * S] = total;// 最後埋め

		// 埋めなおし
		edges.clear();
		edges.reserve(total);
		for (std::uint32_t i = 0; i < N * S; ++i)
		{
			edges.insert(edges.end(), buckets[i].begin(), buckets[i].end());//
		}
	}


	CanonicalFSM FSMRegistry::makeCanonicalFSM
	(
		const FSMDTO& fsm,
		const AxisTable& at,
		const FSMCheck& chk,
		BuildErrors& err
	)
	{
		CanonicalFSM cfsm{};
		cfsm.name = fsm.fsm;
		cfsm.numStates = (std::uint32_t)fsm.states.size();
		cfsm.numSlots = (std::uint32_t)at.slotOrder.size();
		cfsm.numProfiles = (std::uint32_t)fsm.profiles.size();
		cfsm.version = fsm.version;

		auto it = at.condByName.find(kBuiltinAlwaysTrue);
		if (it != at.condByName.end())
		{
			cfsm.alwaysTrueBit = it->second.v;
		}

		// condOf
		ExtendsCheck ec;
		(void)checkProfileDefs(fsm, at, err, ec);
		buildCondOf(fsm, at, ec, fsm.profiles, cfsm.condOf);

		// CSR
		std::unordered_map<std::string, std::uint32_t> stateLocalIndex;
		for (std::uint32_t i = 0; i < fsm.states.size(); ++i) stateLocalIndex.emplace(fsm.states[i], i);
		buildCSR(fsm, at, stateLocalIndex, cfsm.ofs, cfsm.edges);

		// local -> global
		cfsm.local2GlobalState = chk.local2GlobalState;
		cfsm.local2GlobalProfile = chk.local2GlobalProfile;
		cfsm.local2GlobalSlot.resize(at.slotOrder.size());
		for (std::uint32_t s = 0; s < at.slotOrder.size(); ++s)
		{
			cfsm.local2GlobalSlot[s] = at.slotByName.at(at.slotOrder[s]);
		}

		return cfsm;
	}

	void BakeEnvAssemblerPlan
	(
		const CanonicalAxis& ax,
		AxisRuntime& rt,
		FieldResolver  resolveField,
		BuildErrors& err
	)
	{
		rt.plan.ops.clear();
		const auto& specs = ax.derivedSpecs;

		for (const auto& s : specs)
		{
			if (s.kind == CondKind::Bit)
			{
				continue;
			}

			EnvAssemblerPlan::Op op{};
			op.kind = s.kind;
			op.op = s.op;
			op.f32_0 = s.f32_0;
			op.f32_1 = s.f32_1;
			op.u16_0 = s.u16_0;
			op.outBitIndex = s.outBitIndex;

			// fieldName -> field index を解決
			std::uint16_t idx = 0;
			if (!resolveField || !resolveField(s.fieldName, idx))
			{
				err.err("Axis '" + ax.axisName + "': unknown field '" + s.fieldName + "' for condBit " + std::to_string(s.outBitIndex));

				continue;
			}

			op.field = idx;

			rt.plan.ops.push_back(op);
		}
	}

}