# design-notes/ README

## このフォルダの目的
- **役割**: 設計の「考え方・枠組み・判断基準」を**半体系化**して記録する場所。最終仕様（`specs/`）より前、思考メモ（`dev-thoughts/`）より後の粒度。
- **成果物**: 観測可能なゴール、データ契約、フレーム順序、調停規則、受け入れ条件など**再現可能性**を担保する記述。
- **対象**: ドメイン一般の設計指針と、エンジン/ゲーム双方に通用する抽象モデル。具体API/クラス名への依存は避ける。

## 隣接フォルダとの関係
- `dev-thoughts/`：思考のスケッチ。仮説・未検証多め。**着想の源泉**。
- `design-notes/`（本フォルダ）：着想を**構造化**。観測点と調停ルールを明記。
- `adr/`：重大な方針を**採択**する最終判断ログ。設計ノートの結論がここへ昇格。
- `specs/`：最終仕様。**インターフェース定義とテスト条件**が固定化。

> 原則：**dev-thoughts → design-notes → ADR → specs** へ段階昇格。  
> 段階を飛ばす場合は理由を明記。

## 文書のライフサイクルと命名
- **ファイル名**: `Topic_Name.md`（英語推奨、固有名詞はキャメル/パスカルOK）
- **ステータス**: `Draft → Review → Final`  
  - *Draft*: 仮説を構造化。反証歓迎  
  - *Review*: 検証中。PoC/ログ/テストスナップショット付与  
  - *Final*: ADRに吸い上げ済み or 仕様へ反映済み
- **メタ情報**（各ノート先頭）:
  - `Stats` / `Owner` / `Related ADR` / `Related Code/PR`

## 記述スタイル（最小テンプレ）
設計ノートは以下の章立てを**必須**とする（VS Code スニペット `dnote` を推奨）。

- Problem（1–2行で課題の本質）
- Goals / Non-goals（観測可能なゴール・スコープ外）
- Context & Assumptions（前提・制約）
- Interfaces（Events / Requests / Replies / Side-effects keys）
- Flow / Ordering（1フレーム順序、キュー寿命、重複抑止）
- Invariants（決定性・冪等・再入安全など 3点以内）
- Arbitration / Priority（採択規則）
- Prototype Plan（薄い縦切り 1–2本）
- Acceptance Criteria（ログ/テストで観測可能）
- Open Questions（ADR候補）

> **観測可能性**（ログキー、テスト断面、再現手順）を必ず記載。

## 中核コンセプト（抽象化）
### 1) FSMを「意味的な制御単位」として扱う
- FSM＝「状態を制御するための**意味ラベル**と調停規則をもつ枠組み」
- 副作用（Side-effects）＝「状態/遷移に伴う**書き込み系処理**（ゲーム世界への反映）」
- FSMは**常駐型（Reactive）**と**トリガー型（Process）**の二系統で捉える

| 分類 | 説明 | 代表的な用途 | 特徴 |
|---|---|---|---|
| 常駐型FSM（リアクティブ制御） | 常に意味のある状態に滞在し、毎フレーム評価される | 移動/アニメ/入出力仲介など | 状態が恒常・副作用は連続 |
| トリガー型FSM（プロセス制御） | 外部契機で起動し、有限時間の挙動を進行 | スキル/アクション/演出など | 状態遷移が「意味の発生点」 |

### 2) 副作用の整理軸（ドメイン非依存）
**トリガー軸**（いつ実行されるか）
- `OnEnter(state)` / `OnExit(state)` / `WhileIn(state)` / `OnTransition(from→to)` / `AlwaysWhileFSMActive`

**スコープ軸**（どこに効くか）
- `State` / `Transition` / `FSM` / `Global`（Entity全体や他サブシステム含む）

**優先・上書きモード**
- 同一対象に複数制御が作用しうるため、**優先順位**と**合成/上書き**を宣言する  
  - 例：`override`（優先序列勝ち/最後勝ち）、`additive`（加算/乗算合成）

### 3) 外部トリガーと直交FSM
- **外部イベント起点**（入力、被弾、タイマーなど）と、**他FSM状態起点**（直交参照）の二経路
- 直交参照では「状態への干渉」と「副作用への干渉」を区別してモデル化する

## レビュー観点（チェックリスト）
- **決定性**：同一入力列で出力は一意か（処理順序は仕様で固定されているか）
- **冪等性**：同一フレーム中の同一キー適用は二重反映しないか
- **調停規則**：`priority > kind(Hard/Soft) > scope > timestamp > issuerId` 等の**全順**が定義済みか
- **スコープ/トリガー**：どの副作用がどの軸で発火・適用・解除されるか明示されているか
- **観測可能性**：ログキー（`requestId`, `causeId` など）とテスト断面が定義されているか
- **昇格可能性**：Open Questions が ADR 候補として抽出されているか

## 変更ルール
- 破壊的変更（Arbitration規則やフレーム順序の変更）は**PRにレビュータグ**（`design-breaking-change`）を必須。
- ノート末尾に **Changelog** を任意で付け、日付・要点・影響範囲を列挙。
- Final化時は**関連ADR**と**specs/反映**を更新。

## 例示の扱い
- 例は**抽象名**（`StateA`, `EffectX`, `EventY`）を用いる。具体クラス/関数名は避ける。
- 実コード断片は `Related Code/PR` へのリンクに留め、本文には置かない。

## 新規ノートの作り方（最短手順）
1. VS Code スニペット `dnote` で最小テンプレを起こす  
2. **Flow/Ordering** と **Arbitration** を先に書く（決定性の土台）  
3. **Interfaces** を列挙（型名は抽象、フィールドは観測可能なキー中心）  
4. **Acceptance Criteria** を先に置く（検証容易性の確保）  
5. レビューで Open Questions を ADR 候補へ昇格

## 用語（簡易グロッサリー）
- **制御（Control）**：ゲーム的意味に対する主たる操作（何をどう動かすか）
- **副作用（Side-effect）**：制御の結果として行われる**書き込み系処理**（見た目/物理/状態値など）
- **Impulse / Sustain**：瞬間適用 / 継続適用（解除契機や寿命管理が異なる）
- **Hard / Soft**：強制干渉 / 緩和干渉（調停時の優先度種別）
- **Interference**：直交FSM・外部イベントからの状態/副作用への干渉要求

---

### 付録：最小スケルトン(コピペ可) 
```md
# <Design-notes ${タイトル}>
- **Stats**: Draft
- **Owner**: @you
- **Related ADR**:
- **Related Code/PR**:

## Problem
1–2行で記述。

## Goals / Non-goals
- Goals: 観測可能な3–5点
- Non-goals: 今回やらないこと

## Context & Assumptions
前提・制約を箇条書き。

## Interfaces (Data Contracts)
- Events: `Name{fields...}` — 役割1行
- Requests/Replies: `Name{fields...}` → `Reply{fields...}`
- Side-effects keys: `key : type`（冪等性メモ）

## Flow / Ordering
A → B → C  
Queue寿命（frame-bounded等）/ 重複抑止条件を1行。

## Invariants
決定性・冪等・再入安全など 3点以内。

## Arbitration / Priority
採択規則（例：priority > timestamp > issuerId）。

## Prototype Plan
1) 薄い縦切りA（完了条件1行）
2) 薄い縦切りB（完了条件1行）

## Acceptance Criteria
ログ/テストで観測可能な3–5点。

## Open Questions
未決事項（将来ADR候補）。
```