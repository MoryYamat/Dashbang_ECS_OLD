#pragma once

#include "Entity.h"

#include "ComponentFilter.inl"

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <type_traits>
#include <vector>
#include <memory>
#include <functional>

#include <cstddef>
#include <iostream>

#include <cassert>



// TODO: 最適化
// 
// 破棄フック
// remove / destroy / clearで使う
// viewの最適化
// 薄いファザード実装
//
namespace Engine::ECS
{
	// Entity Manager
	class EntityMgr
	{
	private:

		uint32_t nextEntityID = 1;

		// ※※Efficiency issues※※	
		// ※※Efficiency issues※※
		// ※※Efficiency issues※※
		// コンポーネントごとにエンティティを紐づける
		std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>> mComponentPools;
		// {type_index(Component), unordered_map{e.id, Components.data}}

	/// <summary>
	/// mComponentPools = {
	/// 	{ typeid(TransformComponent), { 1: data1, 3: data3 } },
	/// 	{ typeid(LogicComponent) , { 1: data1 } },
	/// 	{ typeid(AIComponent) , { 2: data2, 5: data5 } }
	/// </summary>

		// 
		std::unordered_set<uint32_t> mLivingEntities;

		// グローバルに存在する参照すべき共有情報
		std::unordered_map<std::type_index, std::shared_ptr<void>> mResources;

		struct TypeInfo
		{
			std::function<void(void*)> onDestroy;// raw ptr -> sharedの中身
		};

		std::unordered_map<std::type_index, TypeInfo> mTypeInfo;


		template<typename T>
		static void CallDestroyIfExists(void* raw)
		{
			if constexpr (requires(T & x) { x.Destroy(); })
			{
				static_cast<T*>(raw)->Destroy();
			}
		}

	public:
		// create
		Entity createEntity()
		{
			Entity e{ nextEntityID++ };
			mLivingEntities.insert(e.id);
			return e;
		}

		// add
		//template<typename T>
		//T& addComponent(Entity e, T&& component)
		//{
		//	using U = std::decay_t<T>;
		//	const std::type_index type = std::type_index(typeid(U));

		//	auto& bucket = mComponentPools[type];
		//	auto sp = std::make_shared<U>(std::forward<T>(component)); // ムーブ or コピー（コピー禁⽌ならムーブだけ使う）
		//	bucket[e.id] = sp;

		//	return *std::static_pointer_cast<U>(sp);
		//}
		template<typename T, typename... Args>
		T& addComponent(Entity e, Args&&... args)
		{
			using U = std::decay_t<T>;
			const std::type_index type = std::type_index(typeid(U));

			assert(isAlive(e) && "addComponent: dead entity");

			// 初回だけ破棄フック登録
			if (mTypeInfo.find(type) == mTypeInfo.end())
			{
				mTypeInfo.emplace(type, TypeInfo{
					[](void* raw) { CallDestroyIfExists<U>(raw);}
				});
			}

			auto& bucket = mComponentPools[type];
			
			if (auto it = bucket.find(e.id); it != bucket.end() && it->second)
			{
				if (auto ti = mTypeInfo.find(type); ti != mTypeInfo.end())
					ti->second.onDestroy(it->second.get());
			}

			auto sp = std::make_shared<U>(std::forward<Args>(args)...);
			bucket[e.id] = sp;

			return *std::static_pointer_cast<U>(sp);
		}

		//template<typename T>
		//void addComponent(Entity e, T component)
		//{
		//	const std::type_index type = std::type_index(typeid(T));

		//	if (mComponentPools.find(type) == mComponentPools.end())
		//	{
		//		//{ EntityID, ... }
		//		mComponentPools[type] = std::unordered_map<uint32_t, std::shared_ptr<void>>();
		//	}

		//	mComponentPools[type][e.id] = std::make_shared<T>(component);
		//}

		// get
		template<typename T>
		T& get(Entity e)
		{
			const auto key = std::type_index(typeid(T));
			auto it = mComponentPools.find(key);
			assert(it != mComponentPools.end());

			auto jt = it->second.find(e.id);
			assert(jt != it->second.end() && jt->second);

			return *static_cast<T*>(jt->second.get());
		}

		// writable
		template<typename T>
		T* tryGet(Entity e) noexcept
		{
			const auto key = std::type_index(typeid(T));
			auto it = mComponentPools.find(key);
			if (it == mComponentPools.end()) return nullptr;

			auto& cmap = it->second;
			auto cit = cmap.find(e.id);
			if (cit == cmap.end() || !cit->second) return nullptr;

			return static_cast<T*>(cit->second.get());
		}

		// read only
		template<typename T>
		const T* tryGet(Entity e) const noexcept
		{
			const auto key = std::type_index(typeid(T));
			auto it = mComponentPools.find(key);
			if (it == mComponentPools.end()) return nullptr;

			const auto& cmap = it->second;
			auto cit = cmap.find(e.id);
			if (cit == cmap.end() || !cit->second) return nullptr;

			return static_cast<T*>(cit->second.get());
		}

		// search and safety valve
		// 検索と安全弁
		template<typename... Components>
		std::vector<Entity> view()
		{
			std::vector<Entity> result;

			using FirstComponent = typename std::tuple_element<0, std::tuple<Components...>>::type;
			const auto key0 = std::type_index(typeid(FirstComponent));
			auto it0 = mComponentPools.find(key0);
			if (it0 == mComponentPools.end()) return result;

			//　事前探索
			const auto& base = it0->second;
			result.reserve(base.size());

			for (const auto& [eid, _] : base)
			{
				bool hasAll = (... && [&] {
					const auto key = std::type_index(typeid(Components));
					auto it = mComponentPools.find(key);
					return it != mComponentPools.end() && it->second.count(eid) > 0;

				}());

				if (hasAll)
				{
					result.push_back(Entity{ eid });
				}
			}

			return result;

		}

		// viewに使用：FilterSpec構造体：{std::taple<MustComps...>, std::tuple<AnyComps...>}
		template<typename MustSet, typename AnySet>
		struct FilterSpec {};

		//using Must = std::tuple<TransformComponent, LogicComponent>;
		//using Any = std::tuple<AIComponent, PhysicsComponent>;
		//auto result = ecs.view(FilterSpec<Must, Any>{});
		// Mustだけ渡すとAND検索 Anyだけ渡すとOR検索 両方渡すとAND + OR検索　
		template<typename... MustComponents, typename... AnyComponents>
		std::vector<Entity> view(FilterSpec<std::tuple<MustComponents...>, std::tuple<AnyComponents...>>)
		{
			std::vector<Entity> result;

			using FirstMust = std::conditional_t<
				sizeof...(MustComponents) != 0,
				typename std::tuple_element<0, std::tuple<MustComponents...>>::type,
				void>;

			if constexpr (!std::is_same_v<FirstMust, void>)
			{
				std::type_index baseType = std::type_index(typeid(FirstMust));
				auto baseIt = mComponentPools.find(baseType);
				if (baseIt == mComponentPools.end()) return result;

				const auto& basePool = baseIt->second;

				for (const auto& [entityID, _] : basePool)
				{
					bool hasAll = ::Engine::ECS::Detail::template hasAllComponents<MustComponents...>(mComponentPools, entityID);
					bool hasAny = ::Engine::ECS::Detail::template hasAnyComponents<AnyComponents...>(mComponentPools, entityID);
					//bool hasAll = (... && [&]() {
					//	auto it = mComponentPools.find(std::type_index(typeid(MustComponents)));
					//	return it != mComponentPools.end() && it->second.count(entityID) > 0;
					//	}());

					//bool hasAny = (... || [&]() {
					//	auto it = mComponentPools.find(std::type_index(typeid(AnyComponents)));
					//	return it != mComponentPools.end() && it->second.count(entityID) > 0;
					//	}());

					if (hasAll && (sizeof...(AnyComponents) == 0 || hasAny))
					{
						result.push_back(Entity{ entityID });
					}
				}
			}
			else
			{
				// Mustが空 → Anyだけで走査（OR検索）
				if (mComponentPools.empty()) return result;

				const auto& basePool = mComponentPools.begin()->second;

				for (const auto& [entityID, _] : basePool)
				{
					bool hasAny = (... || [&]() {
						auto it = mComponentPools.find(std::type_index(typeid(AnyComponents)));
						return it != mComponentPools.end() && it->second.count(entityID) > 0;
						}());

					if (hasAny)
					{
						result.push_back(Entity{ entityID });
					}
				}
			}

			return result;

			//std::vector<Entity> result;

			//using FirstMust = std::conditional_t<
			//	sizeof...(MustComponents) != 0,
			//	typename std::tuple_element<0, std::tuple<MustComponents...>>::type,
			//	void>;

			//if constexpr (!std::is_same_v<FirstMust, void>)
			//{
			//	std::type_index baseType = std::type_index(typeid(FirstMust));
			//	if (mComponentPools.count(baseType) == 0) return result;
			//	const auto& basePool = mComponentPools.at(baseType);

			//	for (const auto& [entityID, _] : basePool)
			//	{
			//		bool hasAll = Detail::hasAllComponents<MustComponents...>(mComponentPools, entityID);
			//		bool hasAny = Detail::hasAnyComponents<AnyComponents...>(mComponentPools, entityID);
			//		if (hasAll && (sizeof...(AnyComponents) == 0 || hasAny))
			//		{
			//			result.push_back(Entity{ entityID });
			//		}
			//	}
			//}
			//else
			//{
			//	// Mustが空の場合 -> Anyで走査 (OR検索のみ)
			//	if (mComponentPools.empty()) return result;
			//	// 任意のプールを一つ取得
			//	const auto& basePool = mComponentPools.begin()->second;

			//	for (const auto& [entityID, _] : basePool)
			//	{
			//		bool hasAny = Detail::hasAnyComponents<AnyComponents...>(mComponentPools, entityID);
			//		if (hasAny)
			//		{
			//			result.push_back(Entity{ entityID });
			//		}
			//	}
			// return result;
			//}
		}

		// Entityが該当Componentを持っているか判定する
		template<typename T>
		bool hasComponent(Entity e) const
		{
			// key
			std::type_index type = std::type_index(typeid(T));

			// キーで検索
			auto poolIt = mComponentPools.find(type);
			if (poolIt == mComponentPools.end())
			{
				return false;
			}

			// サブunordered_mapを取得
			const auto& entityMap = poolIt->second;
			// 検索
			return entityMap.find(e.id) != entityMap.end();
		}

		// すべてのComponentに付随する情報の削除
		void Clear();

		// 任意のEntityに付随するComponentの削除(Componentを探索し，該当Entityがあれば削除する)
		void destroyEntity(Entity e)
		{
			for (auto& [type, entityMap] : mComponentPools)
			{
				//std::size_t erased = entityMap.erase(e.id);
				//if (erased > 0)
				//{
				//	std::cout << "[EntityManager(destroy)] Deletion of Component """ << type.name() << " for " << e.id << " completed successfully\n";
				//}

				auto it = entityMap.find(e.id);
				if (it != entityMap.end() && it->second)
				{
					if (auto ti = mTypeInfo.find(type); ti != mTypeInfo.end())
						ti->second.onDestroy(it->second.get());
					entityMap.erase(it);
				}
				
			}
			mLivingEntities.erase(e.id);
		}

		bool isAlive(Entity e) const
		{
			return mLivingEntities.count(e.id) > 0;
		}

		// Resource 登録・取得API

		// Resourceを登録
		//template<typename T>
		//void addResource(const T& resource)
		//{
		//	mResources[std::type_index(typeid(T))] = std::make_shared<T>(resource);
		//}

		// Resourceを登録
		//template<typename T, typename... Args>
		//T& createResource(Args&&... args)
		//{
		//	// インスタンス生成
		//	auto res = std::make_shared<T>(std::forward<Args>(args)...);
		//	// 新規作成
		//	mResources[std::type_index(typeid(T))] = res;
		//	// shared_ptr<T>を返す
		//	return *res;
		//}

		template<typename T, typename... Args>
		T& createResource(Args&&... args)
		{
			const auto key = std::type_index(typeid(T));
			auto it = mResources.find(key);
			if (it != mResources.end())
			{
				return *std::static_pointer_cast<T>(it->second);
			}
			auto sp = std::make_shared<T>(std::forward<Args>(args)...);
			mResources.emplace(key, sp);
			return *sp;
		}


		// mResourceを取得
		template<typename T>
		T& getResource()
		{
			const auto key = std::type_index(typeid(T));
			auto it = mResources.find(key);

			assert(it != mResources.end());
			return *std::static_pointer_cast<T>(it->second);
		}

		template<typename T>
		const T& getResource() const
		{
			const auto key = std::type_index(typeid(T));
			auto it = mResources.find(key);

			assert(it != mResources.end());
			return *std::static_pointer_cast<T>(it->second);
		}

		// Resouceが存在するか確認
		template<typename T>
		bool hasResource() const
		{
			return mResources.count(std::type_index(typeid(T))) > 0;
		}

		template<typename T, typename... Args>// パラメータパック
		T& getOrCreateResource(Args&&... args)
		{
			const auto key = std::type_index(typeid(T));
			auto it = mResources.find(key);
			if (it != mResources.end())
			{
				return *std::static_pointer_cast<T>(it->second);
			}
			auto sp = std::make_shared<T>(std::forward<Args>(args)...);// 完全転送=> {std::forward<int>(value1), std::forward<hoge>(value2),..} //forward:値カテゴリ(右辺値／左辺値)をそのまま次に渡す
			auto [insIt, _] = mResources.emplace(key, sp);// 構造化束縛
			return *std::static_pointer_cast<T>(insIt->second);
		}

		// 任意のComponentを削除するAPI
		template<typename T>
		void removeComponent(Entity e)
		{
			std::type_index type = std::type_index(typeid(T));

			auto poolIt = mComponentPools.find(type);
			if (poolIt == mComponentPools.end()) return;

			auto& entityMap = poolIt->second;
			auto it = entityMap.find(e.id);
			if (it != entityMap.end() && it->second)
			{
				if (auto ti = mTypeInfo.find(type); ti != mTypeInfo.end())
					ti->second.onDestroy(it->second.get());
				entityMap.erase(it);
			}


			// entityMapが空ならtype自体の登録を削除してもよい（最適化）
			if (entityMap.empty())
			{
				mComponentPools.erase(type);
			}
		}

	};
}

