#pragma once

#include "Engine/ECS/EntityManager.h"

#include <vector>
#include <memory>
#include <utility>

// 各ドメインには 必要に応じて wrapper を作成

// ecs用ファサード
namespace Engine::ECS::Ops
{
	// Entity Create
	inline Engine::ECS::Entity Create(Engine::ECS::EntityMgr& ecs)
	{
		return ecs.createEntity();
	}

	// destroy
	inline void Destroy(Engine::ECS::EntityMgr& ecs, Entity e)
	{
		ecs.destroyEntity(e);
	}

	// add (完全転送)
	template<typename T, typename... Args>
	inline T& Add(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity e, Args&&... args)
	{
		return ecs.addComponent<T>(e, std::forward<Args>(args)...);
	}

	// 取得
	template<typename T> inline T& Get(Engine::ECS::EntityMgr& ecs, Entity e){ return ecs.get<T>(e);}
	template<typename T> inline T* TryGet(Engine::ECS::EntityMgr& ecs, Entity e) { return ecs.tryGet<T>(e); }
	template<typename T> inline bool Has(Engine::ECS::EntityMgr& ecs, Entity e) { return ecs.hasComponent<T>(e); }

	// remove
	template<typename T>
	inline void Remove(Engine::ECS::EntityMgr& ecs, Entity e)
	{
		ecs.removeComponent<T>(e);
	}

	// Global Resource
	template<typename T>
	inline T& CreateResouce(Engine::ECS::EntityMgr& ecs)
	{
		return ecs.createResource<T>();
	}
	
	template<typename T>
	inline T& GetRes(Engine::ECS::EntityMgr& ecs)
	{
		return ecs.getResource<T>();
	}

	template<typename T, typename... Args>
	inline T& GetOrCreateRes(Engine::ECS::EntityMgr& ecs, Args&&... args)
	{
		return ecs.getOrCreateResource<T>(std::forward<Args>(args)...);
	}

	// view(AND 検索) 
	template<typename... Components>
	inline std::vector<Engine::ECS::Entity>
		View(Engine::ECS::EntityMgr& ecs)
	{
		return ecs.template view<Components...>();
	}


	// view (Must/Any フィルタ) の素通し
	// Ops::ViewFiltered<Must..., Any...>(ecs)
	//template<typename... Must, typename... Any>
	//inline std::vector<Engine::ECS::Entity>
	//	ViewFiltered(Engine::ECS::EntityMgr& ecs)
	//{
	//	using Filter = typename Engine::ECS::EntityMgr::template FilterSpec<
	//		std::tuple<Must...>, std::tuple<Any...>>;
	//	return ecs.view(Filter{});
	//}

	// AND 検索 -> (e, comp1, comp2, ...) をコールバックへ// Ops::ForEach<TransformComponent, MeshComponent>(ecs, [](Entity e, auto& tr, auto& mesh){});
	template<typename... Components, typename F>
	inline void ForEach(Engine::ECS::EntityMgr& ecs, F&& fn)
	{
		for (auto e : ecs.template view<Components...>())
		{
			std::forward<F>(fn)(
				e, 
				(ecs.template get<Components>(e))...
			);
		}
	}

	//template<typename... Must, typename... Any, typename F>
	//inline void ForEachFiltered(Engine::ECS::EntityMgr& ecs, F&& fn) {
	//	using Filter = Engine::ECS::EntityMgr::FilterSpec<
	//		std::tuple<Must...>, std::tuple<Any...>>;
	//	for (auto e : ecs.view(Filter{})) {
	//		std::forward<F>(fn)(e, 
	//			(ecs.template get<Must>(e))...
	//		);
	//		// Any は「どれか1つでもあればOK」なので、必要なら TryGet で個別処理
	//	}
	//}

}