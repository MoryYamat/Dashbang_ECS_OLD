#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"


namespace Game::FSM
{
	bool InitAllFSMs_Game(Engine::WorldSystem::Core::WorldCtx& ctx);

}


// 次の小仕上げ（短め）
// 
// 1. ランナー最小実装
// 現在状態とプロフィールを保持して、毎フレーム env を更新→DecideNext_Slots で評価。
// 
// struct AxisRunner {
//     const CanonicalAxis* ax{};
//     const CanonicalFSM* f{};
//     AxisRuntime* rt{};
//     uint32_t state{};
//     uint32_t profile{};
// 
//     bool step(const std::span<const uint32_t> slots,
//         const EnvSnapshot& env, const EvalCtx& ctx) {
//         auto d = DecideNext_Slots(*f, *rt, state, profile, slots, env, ctx);
//         state = d.to;
//         return d.changed;
//     }
// };
// 
// 取得は db.get("Movement")→rt->canon->fsms.front() と findIdx で state / profile を初期化。
// 
// 2.インデックス解決ヘルパ
// 名前↔インデックスの往復を関数化（ログやデバッグに便利）。
// 
// 3. 失敗の早期検出
// 
// BindAxisConds で unknown / nullptr / 未バインドがあれば false を返す（今の実装でOK）。
// 
// InitAllFSMs_Game の戻り値を必ずチェックしてフェイルセーフへ。
// 
// 4. 超簡単ユニットテスト
// 
// Idle + Transition で Moving
// 
// Moving + Stop で Idle
// 
// canMove = false で遷移しない
// 
// 5. 優先度（prio）の降順評価確認
// 
// ログの名前化
// local2Global * と axis.*Order を使って from / to / slot / profile / cond を文字列で出せるヘルパを1つ。