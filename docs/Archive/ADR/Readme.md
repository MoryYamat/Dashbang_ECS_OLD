# ADR (Architecture Decision Record)

このディレクトリは、**設計上の重要な意思決定を短く記録**する場所です。長文の議論や思索は `dev-thought/` に残し、**最終の決定だけ**をここに保存します。



## 目的 (why)
- 後からなぜそうしたかを追えるようにする
- レビュー・将来の変更の起点を作る
- 議論ログ(dev-thought)と**決定**を分離する

## いつ書く(Trigger)
- **不可逆／高コスト**の変更(データ形式，永続化，ネットワーク同期，アロケータなど)
- **長寿命のインターフェース** (FSM間リクエスト，効果／副作用チャンネルなど)
- **横断影響が大きいもの** (入力，ビルド/CI，物理，認証)
- **性能／安全の予算**にかかわるもの (フレーム／メモリ／セキュリティ)


> 上記に当てはまらないものは PR の説明や dev-thougt で十分


## ファイル命名規則 (Naming)
```text
ADR-YYYY-MM-DD-<slug>[ -<suffix>].md
```

- `YYYY-MM-DD`：決定日（提案日でOK）
- `<slug>`：**kebab-case**、3〜6語、強い名詞・ドメイン名を含める（例：`fsm-priority-policy`, `world-query-injection`）
- `<suffix>`：**同日に同テーマが複数**ある場合のみ `-a/-b` か `-02` などを付与

**例**
- `ADR-2025-08-13-fsm-priority-policy.md`
- `ADR-2025-08-13-world-query-injection.md`
- `ADR-2025-08-13-fsm-arbiter-interface-a.md` / `ADR-2025-08-13-fsm-arbiter-interface-b.md`

# <タイトル（決定の短い要約）>

- **Status**: <Proposed | Accepted | Superseded by #ID | Deprecated>
- **Date**: YYYY-MM-DD
- **Owner**: @you
- **Tags**: <カンマ区切り例: FSM, Priority, Arbitration>
- **Component**: <サブシステム名例: Combat/SkillFSM>
- **Related**: dev-thought/<path>

## Context
なぜ今これが必要か（1〜2行）

## Decision
何をどう決めたか（1〜2行）※「する/しない」を明示

## Consequences
主な効果・トレードオフ・影響範囲（1〜2行）

## Verification
どう検証するか（ログ/テスト観測点を1行）

## Review
見直し条件 or 期限（例：2025-08-25、または条件）

---

## 2) ステータス（Lifecycle）

- ~~`Proposed`（提案中）→ レビュー → `Accepted`（採用）~~
- ~~置換時：`Superseded by ADR-YYYY-MM-DD-...`~~
- ~~廃止：`Deprecated`（歴史参照用に残す）~~
- ~~却下：`Rejected`（重要な検討の足跡として残す場合のみ）~~

---

## 3) 本文テンプレート（Content Template）

> 目安：各セクション **1〜3行**。詳細は dev-thought に置く。  
> **検証（Verification）**は1行で必ず書く。