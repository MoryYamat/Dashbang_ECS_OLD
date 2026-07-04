## **Skill System**(2025/06/04まで)**意図層の導入により変更**
* 本SkillSystemは、入力からスキルの発動・消滅に至るまでの一連の処理を、ECSベースで柔軟かつ拡張可能に設計したスキル管理基盤である。

### **システムの概要**
> 入力（InputAction）→ スロット（SkillSlot）→ スキル（SkillId）→ 発動（Entity）
という段階的解決を経て、スキルの即時発動が行われる。


### **処理フロー**
```text
[InputAction] ──▶        [SkillSlot]
       ▲                       │
       │                       ▼
[InputActionComponent]  [SkillSlotAssignmentComponent]
       │                  │
       └────── SkillInputMap ───┐
                                ▼
                       [SkillTriggerSystem]
                                │
                                ▼
                     Create [SkillInstanceComponent]
                                │
                                ▼
                     [SkillCastingSystem]
                                │
                                ▼
                     Generate Attack2DArea
                                │
                                ▼
                     [SkillLifetimeSystem]
                                │
                                ▼
                  Auto-destroy expired entities

```
#### **各ステージの説明**
|ステージ|	内容|
|:-|:-|
|InputAction|	GLFWなどによる生の入力定義（マウス・キーボード）|
|SkillInputMap|	InputAction → SkillSlot のマッピング（バインディング）|
|InputActionComponent|	各プレイヤーが今押しているアクション（フレームごとに更新）|
|SkillSlotAssignmentComponent|	各プレイヤーに対応する SkillSlot → SkillId 割り当て情報|
|SkillTriggerSystem|	押されている InputAction に基づき、プレイヤーのスロット → スキルID を引いてスキル発動をトリガー（エンティティ生成）|
|SkillInstanceComponent|	スキルが発動中であることを記録するエンティティコンポーネント|
|SkillCastingSystem|	SkillDefinition を取得し、実際の攻撃判定などを生成|
|SkillLifetimeSystem|経過時間を監視し、スキルエンティティを削除する|

### **設計の特徴**
* データと処理の分離
* 汎用的なトリガー設計
* データ駆動設計 `SkillInputMap`による柔軟なスキルバインドの実装
* 処理レイヤの分離 入力・発動・定義・終了が完全に責務分離している

## **キャラクター状態との連携**
1. キャラクター状態 -> スキル状態 ： `依存` キャラ状態がスキル使用可能性を定義する 
       * 
2. スキル状態 -> キャラクター状態　： `イベント通知` キャラのアニメや行動制限を行う
       * `SkillSystem`は`CharacterSystem`に「直接書き換え」を行わない(状態変更はイベント・フラグを通す)
       * `Skill`はキャラクタの状態を要求するが「所有」しない

### **将来的な拡張**
* 攻撃範囲の動的な変化・移動（移動する攻撃、追尾、広がる範囲など）
* 複合形状スキル(例：範囲+ビームなど)
* キャラクター状態との同期，ヒットイベント，ステータス変化，クールダウン管理などの追加

## **変更**

* 