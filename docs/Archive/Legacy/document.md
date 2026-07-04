## ドキュメント



### コンポーネント初期化に関する補足
```cpp
RunInitializationPhase()// entry
    ApplyAllDeferredInitializations<...>// 遅延初期化したい型を列挙
        ApplyDeferredInitialization<T>() // T型に対して処理
            if GetInitPhase<T>() == Deferred // 必要なものだけInit実行
                for(Entity e: ecs.view<T>())
                    InitSystem<T>::Init(...)// 実際の初期化本体
```

1. `InitPhase`と`GetInitPhase<T>()`
```cpp
enum class InitPhase { Immediate, Deferred };

template<typename Component>
constexpr InitPhase GetInitPhase() {
    return InitPhase::Immediate //デフォルト 
}
```


2. `InitSystem<T>`
```cpp
template<typename T>
struct InitSystem{
    static void Init(T& comp, ECS& ecs, Entity owner){
        // default
    }
}
```

3. 特殊化
```cpp
template<>
struct InitSystem<TileMapComponent>{
    static void Init(TileMapComponent& tileMapComp, ECS& ecs, Entity){
        GameInit::TileMapFromMesh::ApplyObstacleCollidersToTileMap(ecs, tileMap);
    }
}
```


4. `ApplyDeferredInitialization<T>()`
```cpp
template<typename T>
void ApplyDeferredInitialization(ECS& ecs){
    if constexpr (GetInitPhase<T>() == InitPhase::Deferred)
    {
        for(Entity e : ecs.view<T>())
        {
            auto& comp = ecs.get<T>(e);
            InitSystem<T>::Init(comp,ecs, e);
        }
    }
}
```

5. `ApplyAllDeferredInitializations<...>`
```cpp
template<typename... Components>
void ApplyAllDeferredInitializations(ECS& ecs)
{
    (ApplyDeferredInitialization<Components>(ecs), ...);
}
```

6. `Game::RunInitializationPhase()`
```cpp
void Game::RunInitializationPhase() {
    ApplyAllDeferredInitializations<
    TileMapComponent,
    FollowCameraComponent
    >(mEcs);
}
```
#### やるべきこと

* 必要な`Component`に対して`InitSystem<T>::Init()`を特殊化する
* `GetInitPhase<T>()`を`Deferred`に設定する
* `ApplyAllDeferredInitializations<T...>()`に型を追加する