# Collision Mask 設計


<details>

## Data
- `Category`: 自分のコリジョン属性を示す            {`Character`, `SkillHitbox`, `Sensor`}(arbitary)
- `Relation`: 衝突対象となる相手との相対関係ラベル   {`None`, `Self`, `Ally`, `Enemy`, `Neutral`, `All`}(set) (当てたい相手が自分から見て`敵か` `味方か` `何か`かを示す)

## CollisionMaskComponent
- `Category` category: 自分のカテゴリ(単一)
- `collideCategoryMask`: 当てたい相手のカテゴリ(集合)
- `relationMask`: 当てたい相手の相対関係ラベル(集合)
- `flags`

## シナリオ
### 1.SkillHitbox (Team A) と CharacterHitbox(Team B)の衝突
0. ({`スキル定義`から`collidewith`/`relation`をコピー}/{スキル副作用で`SpawnHitbox`時に`CharacterEntity`から/`TeamTag`をコピー})
1. → `A`:{`cate: SkillHitbox`, `collidewith: character`, `relation: Enemy, Neutral`}, `TeamTag`: {`1`} 
2. / `B`: {`cate: Character`, `collidewith: character, skillhitbox, (sensor)`, `relation: Ally, Enemy, Neutral`}, `TeamTag`: {`2`}
3. 有効フラグチェック({`flags`})
4. カテゴリ両想いチェック {`A`: {`collidewith: character`} + `B`: {`cate: Character`}} / {`B`: {`collidewith: skillhitbox`}} + {`A`: {`cate: skillhitbox`}}
5. 関係抽出: Aから見たB`rab = Relation::Enemy`/ Bから見たA`rba = Relation::Enemy`
6. `shouldCollide`計算
   1. {(`a.collideCate: chara` & `b.cate: chara`)&&(`b.collideCate: skill` & `a.cate: skill`)}
   2. {(`rela: Enemy`& `rab` ) && (`relb: Enemy`& `rba`)}
   3. `true`
7. 

### 2. CharacterHitbox(A) と CharacterHitbox(B)の衝突


### 3. CharacterHitbox と Obstacle の衝突



</details>