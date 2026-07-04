# collision detection system の設計メモ

> 後回し

## 全体設計

### Engine側とGame側の責務分離

#### Engine側
「衝突エンジン」としての汎用部分  
Gameに依存しない  
- Entity群に対するペア列挙
- 交差判定
- 結果の標準的な出力

#### ペア選択


#### 交差判定



#### Game側
「どのペアをテストするか」「当たったら何をするか」という意味論  
- Category/Relation/Flagsの具体的なenum
- TeamやSkillOwnerComponentを使った「敵・味方・自分」の判定
- HitboxHitMemoComponentなどの契約
- CollisionResultBufferから読み取って、HitEventを生成したり、FSMに通知したりする処理





