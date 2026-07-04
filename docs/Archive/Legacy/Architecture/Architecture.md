## **whole**
1. 実装層()
2. 抽象レイヤ

## **Input System**

### **Layer**

#### **1. Raw Input Layer**
* **構成**：`RawInputState`，`InputManager`
* **責任**：GLFWやデバイスからの物理的なキー，マウス入力を収集する
* **出力**：`std::unordered_map<int, bool>`などの状態

#### **2. InputAction Layer**
* **構成**：`InputAction`，`InputMapping`，`InputActionComponent`
* **責任**：キーコードを意味のあるアクションに変換し，Entityに保持させる
* **例**：`GLFW_KEY_W->InputAction::MoveFoward`

#### **3. Game Logic Layer**
* **構成**：`PlayerControlSystem`，`SkillSystem`など
* **責任**：`InputActionComponent`を参照し，ゲーム内の行動を実行する

### **Data Flow**
```text
[ GLFW ]
　↓ 
[ InputManager -> RawInputState]
　↓
[ InputRouterSystem + InputMapping]
　↓
[ ECS: InputActionComponent]
　↓
[ GameLogic: PlayerMovementSystem など]
```

### **拡張**
* `InputContextManager`によるゲーム状態ごとのバインド切り替え

## **Logic System**
* 