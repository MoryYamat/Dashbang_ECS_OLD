#pragma once

#include "DTO.hpp"
#include "Types.hpp"

#include <unordered_map>
#include <string>
#include <cstdint>
#include <vector>
#include <utility>

namespace Engine::FSM::Core
{
	struct BuildErrors
	{
		std::vector<std::string> msgs;
		bool ok() const { return msgs.empty(); }
		void err(std::string m) { msgs.emplace_back(std::move(m)); }
	};

	struct BuildResult
	{
		FSMCatalog catalog;
		BuildErrors errors;
		bool ok() const { return errors.ok(); }
	};

	enum class BuildStrictness
	{
		Strict,// エラーが1件でもあれば catakog を空にして返す( 誤用防止 )
		Lenient// 可能な範囲で catalogを部分生成して返す(調査用途) 
	};

	// AxisDTO -> 辞書化 ・ 採番
	struct AxisTable
	{
		std::string axisName;
		AxisID id{};
		std::uint16_t version = 1;

		// 名前->ID
		std::unordered_map<std::string, StateID> stateByName;
		std::unordered_map<std::string, CondID> condByName;
		std::unordered_map<std::string, SlotID> slotByName;
		std::unordered_map<std::string, ProfileID> profileByName;

		// ID->名前の逆引き用
		std::vector<std::string> stateOrder;
		std::vector<std::string> condOrder;
		std::vector<std::string> slotOrder;
		std::vector<std::string> profileOrder;
	};

	struct FSMCheck
	{
		std::uint32_t axisRank = UINT32_MAX;
		std::vector<StateID>	local2GlobalState;		// fsm.states
		std::vector<SlotID>		local2GlobalSlot;		// axis.slotOrder
		std::vector<ProfileID>	local2GlobalProfile;	// fsm.profiles
	};

	struct ExtendsCheck
	{
		std::vector<std::uint32_t> topo;
	};

	class FSMRegistry
	{
	private:
		std::vector<AxisDTO> axes_;
		std::vector<FSMDTO> fsms_;

		// 軸名 -> index
		static std::unordered_map<std::string, std::uint32_t>
			indexAxes(const std::vector<AxisDTO>& axes, BuildErrors& err);

		// 軸DTO -> AxisTables(重複チェック & ID採番)
		static AxisTable makeAxisTables(const AxisDTO& a, BuildErrors& err);

		static FSMCatalog assembleCatalogSkeleton(const std::vector<AxisTable>& tables);

		static bool validateFSM
		(
			const FSMDTO& fsm,
			const std::unordered_map<std::string, std::uint32_t>& axisNameToRank,
			const std::vector<AxisTable>& tables,
			BuildErrors& err,
			FSMCheck& out
		);

		// プロファイルの継承を解いて(profile, slot)→CondID の表 condOf を作る処理
		static void buildCondOf
		(
			const FSMDTO& fsm,
			const AxisTable& at,
			const ExtendsCheck& ec,
			const std::vector<std::string>& profileOrder,
			std::vector<CondID>& outCondOf
		);

		// Transitionを CSRに詰める
		static void buildCSR
		(
			const FSMDTO& fsm,
			const AxisTable& at,
			const std::unordered_map<std::string, std::uint32_t>& stateLocalIndex,
			std::vector<std::uint32_t>& ofs,
			std::vector<TransitionEdge>& edges
		);

		// FSM をCanonical化
		static CanonicalFSM makeCanonicalFSM
		(
			const FSMDTO& fsm,
			const AxisTable& at,
			const FSMCheck& chk,
			BuildErrors& err
		);

	public:
		void add(AxisDTO axis);
		void add(FSMDTO fsm);

		BuildResult build(BuildStrictness policy = BuildStrictness::Strict) const;
	};

}

//
// condOf（プロフィール継承の解決）を組み立てる
// 
// 遷移を(state, slot) マトリクスで CSR に詰める（ofs / edges）
// 
// local2Global * を埋めて CanonicalAxis.fsms に push
