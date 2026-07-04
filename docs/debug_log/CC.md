# CC 系



## 目次
- [CCFSMSystem/Req動作確認](#debug_cc_req)



<details>
<summary id="debug_cc_req"> <strong>CCFSMSystem/Req動作確認</strong> </summary>

### デバッグ (2025/09/18 時点)

### 想定仕様
- `window`は一回目のCC適用が開始点のスライディング方式(固定区間ではない！)
- 同型CCの上書きは可能 (`count>=threshold`にならない限り)
- `IMMUNE`発火は`count>=threshold`になったのち直近CC解除時点で適用

### 不変条件
- I1: `IMMUNE`は「window内で`count>=2`」かつ「CC効果終了直後」にのみ発火
- I2: `count>=2`の保護期間中はいかなるCCも上書き不可
- I3: `fromNeutral && toCC` はCC適用イベントによって増える

#### ログ解析
```

// -----------------------
// 1回目のCC 被撃 (CC解除予測 13s / window終了 14s , count=1)
[HitEvent] eventID = 1 skillID = 1 target= 1 -> emitCC(type= struct Game::Character::FSM::CC::StateModel::CCState::Stunned, priority= 300)
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::None -> struct Game::Character::FSM::CC::StateModel::CCState::Stunned /clock = 8.64929
[CCFSMSystem] Requesting transition: ->   -> struct Game::Character::FSM::CC::StateModel::CCState::None
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::Stunned -> struct Game::Character::FSM::CC::StateModel::CCState::None /clock = 13.6498
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::None -> struct Game::Character::FSM::CC::StateModel::CCState::Immune /clock = 13.6554
// CC 解除

// === 問題1 (本来はIMMUNEにはならない) ===
[AntiChain] IMMUNE expired -> request NONE /clock = 17.6557
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::Immune -> struct Game::Character::FSM::CC::StateModel::CCState::None /clock = 17.6557
// -----------------------

// -----------------------
// 1回目のCC 被撃 (CC解除予測　24s / window終了 25s , count = 1)
[HitEvent] eventID = 2 skillID = 1 target= 1 -> emitCC(type= struct Game::Character::FSM::CC::StateModel::CCState::Stunned, priority= 300)
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::None -> struct Game::Character::FSM::CC::StateModel::CCState::Stunned /clock = 19.7666
/

// 2回目のCC 被撃 (CC解除予測  26s / window終了 25s , count = 2 )
[HitEvent] eventID = 3 skillID = 1 target= 1 -> emitCC(type= struct Game::Character::FSM::CC::StateModel::CCState::Stunned, priority= 300)
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::Stunned -> struct Game::Character::FSM::CC::StateModel::CCState::Stunned /clock = 21.1836
/

// === 問題2 (本来は上書きされてはいけない) === 
// 3回目のCC 被撃 (CC解除予測 28s / window終了 25s , count = 2)
[HitEvent] eventID = 4 skillID = 1 target= 1 -> emitCC(type= struct Game::Character::FSM::CC::StateModel::CCState::Stunned, priority= 300)
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::Stunned -> struct Game::Character::FSM::CC::StateModel::CCState::Stunned /clock = 23.0169
[CCFSMSystem] Requesting transition: ->   -> struct Game::Character::FSM::CC::StateModel::CCState::None
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::Stunned -> struct Game::Character::FSM::CC::StateModel::CCState::None /clock = 28.0175
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::None -> struct Game::Character::FSM::CC::StateModel::CCState::Immune /clock = 28.0229
/

// 
// 4回目のCC 被撃 (IMMUNE状態は正常に機能している．IMMUNE状態後のCCは受け付けていない)
[HitEvent] eventID = 5 skillID = 1 target= 1 -> emitCC(type= struct Game::Character::FSM::CC::StateModel::CCState::Stunned, priority= 300)
[AntiChain] IMMUNE expired -> request NONE /clock = 32.0232
[CCFSMResolverSystem] Transition applied: struct Game::Character::FSM::CC::StateModel::CCState::Immune -> struct Game::Character::FSM::CC::StateModel::CCState::None /clock = 32.0232
// -----------------------
```

### 問題の挙動
1. `IMMUNE`が`count`にかかわらず発動している
    - 本来`IMMUNE`は`window`単位内で`count>=2`にならないと発動しない
    - 
2. count が CC の上書きに対応できていない
  - `count >= 2`になっても，`Stunned`が上書き可能になっている現象
  - 本来は`count>=2`になれば`Stunned`やその他の`CC`で上書きされてはいけない
- 


### 原因の特定

>[!WARNING] 問題1.について
> **仕様の矛盾**
> - Game/Character/FSM/CC/System/CCFSMResolverSystem.cpp
>  - 仕様：`CCStateComponent`: `current/previous`が**状態更新時のみ変動する**
>
>- Game/Character/Control/CC/System/AntiChainSystem.cpp
>  - 仕様：`line:90`の判定`if(fromNeutral&&toCC)`は**毎フレーム状態更新する前提**
> 

---

>[!Warning] 問題2. について
> **判定の欠如**
> - Game/Character/FSM/CC/System/CCFSMResolverSystem.cpp
>   - `CCStateComponent`の**CC適用前**に`CCAntiChainComponen`の`count`と`threshold`をチェックする必要あり <br>
>   - \-> API化

### 解決策
>[!NOTE] 問題1
> 矛盾の解決
> - Game/Character/FSM/CC/StateModel/CCStateComponent.hpp
>   - `CCTransitionApplied`データを追加し，直近で適用されたリクエストを記録
>   - FSM/CC/System/CCFSMResolverSystem.cppで`state.previous`を毎フレーム更新することによって遷移エッジを検出可能に
>   - Control/CC/System/CCAntiChainSystem.cppで遷移エッジを判定し`anti.count`を計算することで対応

----

>[!NOTE] 問題2
> 判定の追加
> - Game/Character/FSM/CC/System/CCFSMResolverSystem.cpp
>   - `addmisible`計算のラムダ関数に`count`判定を追加

</details>
