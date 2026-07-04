// Data Transfer Object

#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <utility>

#include <unordered_map>

namespace Engine::FSM::Core
{
	inline constexpr const char* kBuiltinAlwaysTrue  = "AlwaysTrue";
	
	// 仕様
	// AlwaysTrue: 恒真命題


	struct CondDefDTO
	{
		std::string cond;		// cond名 (condUの要素と一致)
		std::string kind;		// "Bit" | "CompareF" | "InRange"
		std::string field;		// どの入力フィールドを見るか(movementInputMag)
		std::string op;			// ">", ">=", "<", "<=", "in"
		// 以下は必要な時だけ使う (文字列で受けてbuildでparase)
		std::string th;			// CompareF用の閾値
		std::string low;		// InRange用
		std::string high;		// InRange用
	};


	struct AxisDTO
	{
		std::string axis;						// "Skill"
		std::vector<std::string> stateU;		// 軸全域の状態名
		std::vector<std::string> condU;			// 軸全域の条件名				// 自己ループは禁止(仕様) 無条件スロットアリ
		std::vector<std::string> slotU;			// 軸全域のスロット名			(Slotは評価チャンネル名/ 特定遷移に依存しない条件名)
		std::vector<std::string> profileU;		// 軸全域のプロファイル名
		std::vector<CondDefDTO> condDefs;		
		std::uint16_t version = 1;
	};

	struct TransitionDTO
	{
		std::string from;
		std::string to;
		std::string slot;
		std::uint8_t prio = 0;
	};



	struct ProfileBindDTO
	{
		std::string slot;
		std::string cond;
	};

	struct ProfileDefDTO
	{
		std::string name;
		std::string extends;
		std::vector<ProfileBindDTO> binds;
	};

	// FSM定義 (Axis の要素を仮定してFSMを構成することを想定): DTO時点ではbuild時に整合を確認
	struct FSMDTO
	{
		std::string axis;
		std::string fsm;
		std::vector<std::string> profiles;			// subset
		std::vector<std::string> states;			// subset
		std::vector<ProfileDefDTO> profile_defs;
		std::vector<TransitionDTO> transitions;
		std::uint16_t version = 1;
	};
}

// Use Case (yaml)
// 軸マニフェスト
// 
// axis: Skill
// stateU: [ Cating, Active, Recovery]
// condU: [TimeElapsed, AnimHit, HitCountGte]
// SlotU: [PhaseAdvance, Cancel]
// ProfileU: [Default] 
// version: 1
// -------------------------------------
//
// FSM定義
// fsm: Skill_Phase
// 
// profiles: [Default, Skill_211]
// 
// # トポロジ (構造)
// states: [Casting, Active, Recovery]
// 
// # Profile"束"の宣言(slot->cond)
// profile_defs:
//	Default:
//		PhaseAdvance: TimeElapsed
//		Cancel:		  HitCountGte
//  Skill_221:
//		extends: Default
//		PhaseAdvance: AnimHit
// 
// 
// # 遷移 (構造 + slot)
// transitions:
// - {from: Casting, to: Active, slot: PhaseAdvance, prio: 0}
// - {from: Active, to: Recovery, slot: PhaseAdvance, prio: 0}
//

// Use Case (json)
// {
//	"axis": "Skill",
//	"version": 1,
//	"stateU": ["Casting", "Active", "Recovery"],
//	"condU": ["TimeElapsed", "AnimHit", "HitCountGte"],
//	"slotU": ["PhaseAdvance", "Cancel"],
//	"profiles": {
//		"Default": {"PhaseAdvance": "TimeElapsed", "Cancel": "HitCoutGte"},
//		"Skill_221": {"extends": "Default", "PhaseAdvance": "AnimHit"}
//	},
//	"slotAllowed": {
//		"PhaseAdvance": ["AnimHit", "TimeElapsed"],
//		"Cancel": ["HitCountGte"]
//  },
//	"slotPolicy": { "PhaseAdvance": "open", "Cancel": "closed"},
//	"fsms": [{
//		"name": "Skill_Phase",
//		"states": ["Casting", "Active", "Recovery"],
//		"transitions": [
//			{"from": "Casting", "to": "Active", "slot": "PhaseAdvance", "prio": 0},
//			{"from": "Active", "to": "Recovery", "slot": "PhaseAdvance", "prio": 0}
//		]
//	}]
// }
//

