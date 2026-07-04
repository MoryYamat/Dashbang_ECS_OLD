# 雑記



## 目次


<details>
<summary id="fsm-interference"> <strong>FSMのLeaseによる干渉</strong> </summary>

## なぜLeaseにしたか
FSMの現在の設計・実装構造は以下のようになっている
```
Rule-Layer

StateComponent-Layer

Request-Resolver-Layer

Effect-Layer
```

FSMの直交性を崩さず，定義ドリブンなFSMの干渉を考えた場合<br>
`State-Component`と`Rule`の接続を一時的に弱める(奪う)時限式`Lease`がきれいだと思ったから<br>

普通であれば，`Request-Resolver`に吸収される干渉リクエストを構造的に解決することによって<br>
実装の複雑性を低減できたとおもわれる(今のところは)<br>

### モノイド
`.priority`演算によるモノイドになっている
> [!Important] モノイド
> 1. 演算が閉じている: $A,B \in X \rightarrow A + B = C \in X$ 
> 2. 結合法則が成り立つ: $(A + B) + C = A + (B + C)$
> 3. 単位元の存在: $A + 0 = A$

実装では
- `request` = $\{Stun(.priority = 100, dur = 5s)\}$ / $\{Knockdown(.priority = 200, dur = 3s)\}$
- `.priority`に依存して成立する閉じた演算


### 発展の方向性
- モノイド的な理論を土台にした抽象化
- `.priority`の動的制御(動的モノイド！？)
- 

</details>




$$
PlayerInput \overset{IntentLayer}{\to} \rho \overset{FSMs}{\to} WorldAction \\
\\ 
AIIntent \overset{same\rho}{\to} \rho \overset{FSMs}{\to} WorldAction
$$