# このリポジトリについて
この リポジトリは、 以下のリポジトリの `main` ブランチから取得した古い ZIP のもとに、ローカル環境で動作するよう一部修正したものです．

- 元リポジトリ: https://github.com/MoryYamat/DashBang_ECS
- ベースにしたブランチ: `main`
- ベースコミット: `aad1c52`
- 完全なコミットSHA: `aad1c5282bc47dc054191e25dabf0a1374ce27d4`

以下は古い `README` の内容です．

# **自作3Dゲーム~~エンジン~~**

A fully custom 3D gameplay framework built from scratch in modern C++20,<br>
featuring a data-driven ECS, and a unified intent-based control system.

## **開発環境**
- **Language:** C++20
- **Build:** CMake
- **Rendering:** OpenGL (GLTF-based asset pipeline)
- **Architecture:** Custom ECS + Intent Resolver

--- 
## **マイルストーン**
- **FSM間の干渉(Interference)の完成** (2025/09/25)
- **8軸の移動アニメーション再生実装** (2025/10/09)

## 📺 Demo Video
🎥 [Watch on YouTube (Private Link)](https://www.youtube.com/watch?v=mC9LQ8ZOKVg)
> 自作3Dゲームエンジン上でのロコモーションアニメーション実装。
> FSM制御とヒステリシス処理によって自然な8方向移動を実現。



## **ゲーム基盤**

### **ECS Overview**
All systems are structured under a custom data-oriented Entity Component System.  <br>
Each layer communicates only through data contracts — ensuring modularity, clarity, and testability.



### **Unified Behavior Architecture (Intent-Resolver-FSM Pipeline)**
This engine is built around a **layered input-intent-resolution pipeline**,  <br>
designed for maximum flexibility, debuggability, and behavioral consistency.

```nginx
PlayerInput
   ↓
IntentMappingSystem
   ↓
IntentResolverSystem
   ↓
CharacterFSM
   ↓
GameLogicExecution
   ↓
AnimationResolver
   ↓
Drawing
```

---

🔹 Purpose and Benefits
* Decouple input devices from gameplay logic

* Unify player, AI, and scripted behaviors under a single "Intent" model

* Make command validation explicit and testable

* Enable clean layering of skills, interruptions, and cooldowns

---

🔸 Layer Details
1. **Input → Intent**<br>
Maps raw physical input (keyboard, controller, network) to high-level gameplay intents<br>
such as “MoveForward” or “CastSkill1”.

* Allows flexible remapping and multi-device support

* Cleanly separates physical input from semantic meaning

2. **Intent → Resolver**<br>
Determines whether an intent can be accepted given the current FSM state.

* Example: “Cannot cast while stunned”

* Centralized point for cancel, interrupt, or rejection logic

3. **Resolver → FSM**<br>
Accepts validated intents and drives state transitions.

* Orthogonal FSMs (Movement, Skill, CC) operate concurrently

* Fully data-driven and reusable across actors

4. **FSM → Logic**<br>
Executes gameplay consequences — animations, skill hitboxes, cooldowns, etc.

* Logic derived from state, not hardcoded

* Enables precise timing-based transitions (e.g. Active → Recovery)

---

### ⚙️ **MIC Contract Model (Motion–Intent–Control)**
All gameplay actions are treated as contracts between actor and world.<br>
Intent expresses desire, Control (FSM/Resolver) determines possibility,<br>
and Motion (Animation/Logic) executes the outcome.

|Layer | Role|Resposibility|
|:-|:-|:-|
|**Intent(Request)** | "I want to act."| Declares a desired action (e.g. jump, cast) without<br> assuming feasibility.|
|**Control(Resolver/FSM)**|"Can I act?"| Evaluates state and rules — e.g. can’t jump while<br> airborne, can’t attack while stunned.|
|**Motion(Anim/Logic)**|"Act."|Executes the accepted intent (movement, animation,<br> VFX, hitboxes).|


#### Example
```cpp
Intent { action = JUMP}
→ Resolver: if (FSM.state == Grounded) accept();
             else reject();
→ FSM: Idle → Jumping
→ Animation: play("jump_start")
```

### **Key Idea**
Gameplay is not driven by direct commands but by negotiated contracts<br>
between what an actor wants to do and what the world allows.<br>
This converts scattered conditional checks into explicit, data-driven agreements.<br>

---

### 🧠 Design Philosophy

This engine embodies the principle that gameplay should be state-driven, not input-driven.
Every actor — player, AI, or script — behaves through the same semantic pipeline.<br>

It aims to maximize:

- **Modularity**: Modularity: Systems evolve independently without hidden coupling

- **Scalability**: Scalability: Logic expands via data, not conditional branches

- **Clarity**: Clarity: Intent logs and FSM states make debugging transparent

>(Side note: In real life, intent does not guarantee action — it must pass through constraints
> of state and <br> 
> environment.<br>
> This engine models that exact principle in code.)


#### 🎬 Example Flow: Skill Casting
```pgsql
Player presses [Q]
→ Mapped to `Intent::CastSkill<SkillID>`
→ Resolver checks if current state allows casting
→ FSM transitions: None → Casting
→ Logic: Skill wind-up animation + damage hitbox created
→ After duration, FSM: Casting → Active → Recovery → None
```

---  
### 🔍 Why It Matters

Traditional input-driven systems tightly couple controls and logic, leading to:

- Hard-to-test behaviors (inputs must be simulated)

- Scattered “if” logic (unclear why actions fail)

- Inflexible control schemes (player vs AI divergence)


By introducing Intent and Resolver layers:
- Behavior becomes explicit and state-aware

- Rejections and cancellations are traceable

- AI, human, and scripted entities share one unified behavioral model

---
🧠 In One Line

“Every action in the game is a contract between intent and possibility — a microcosm of will constrained by the world.”

---


### Asset Notice / ライセンス明記

This repository contains 3D character models and animations that include assets
originally obtained from [Mixamo](https://www.mixamo.com/) (© Adobe Systems).

These assets have been combined, modified, and used solely for non-commercial, 
educational, and portfolio purposes to demonstrate game engine and animation system development.

They are **not intended for redistribution, resale, or commercial use**.
If you are an Adobe representative or have any concerns regarding the usage,
please contact me and I will remove or adjust the relevant files immediately.
