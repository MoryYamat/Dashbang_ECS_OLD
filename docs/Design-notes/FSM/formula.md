# FSM構造の形式化





### 定義的枠組み
FSMは有限集合上の遷移写像

$$
M = (Q, \Sigma, \delta, q_0)
$$
として定義される

- Q: 状態集合
- $\Sigma$: 入力集合
- $\delta: Q \times \Sigma \rightarrow Q$（遷移関数）
- $q_0$: 初期状態

### 拡張形式
離散的状態遷移を超えて、連続現象空間$E$を導入する:
$$
\delta^* : (Q \times \Sigma \times T) \rightarrow (Q \times E)
$$

ここで$T$は時間パラメータで、
FSMの遷移と連続的挙動(Effect)を接続する写像構造を表す


### 意味的拡張
FSMの拡張を
$$
f: Q \times T \rightarrow E
$$

とみなすことで、状態(離散)から現象(連続)への意味射として位置付ける
（ラベルを現象へ結び付ける）<br>
→ 離散制御の形式と連続現象制御を統合するための理論的枠組み



