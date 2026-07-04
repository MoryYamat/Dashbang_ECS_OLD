#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/Core/WorldAccess.hpp"

namespace Engine::WorldSystem::Query
{
	// P(e) := ( c_1 ⋀ c_2 ... )
	// AND
	template<typename... Cs>
	struct All
	{
		bool operator()(const Core::WorldRead& rw, Engine::ECS::Core::Entity e) const
		{
			return ((rw.TryGet<Cs>(e) != nullptr) && ...);
		}
	};

	// OR
	template<typename... Cs>
	struct Any
	{
		bool operator()(const Core::WorldRead& rw, Engine::ECS::Core::Entity e) const
		{
			if constexpr (sizeof...(Cs) == 0) return true;
			return ((rw.TryGet<Cs>(e) != nullptr) || ...);
		}
	};


	// L / R について更に合成
	template<typename L, typename R>
	struct And
	{
		L l;
		R r;
		bool operator()(const Core::WorldRead& rw, Engine::ECS::Core::Entity e) const
		{
			return l(rw, e) && r(rw, e);
		}

	};

	template<typename L, typename R>
	struct Or
	{
		L l;
		R r;
		bool operator()(const Core::WorldRead& rw, Engine::ECS::Core::Entity e) const
		{
			return l(rw, e) || r(rw, e);
		}
	};

	template<typename P>
	struct Not
	{
		P p;
		bool operator()(const Core::WorldRead& rw, Engine::ECS::Core::Entity e) const
		{
			return !p(rw, e);
		}
	};


	// ヘルパー関数
	template<typename L, typename R> 
	inline And<L, R> AND(L l, R r) {return { l,r };}// return And<L,R>{l,r}と同じ意味

	template<typename L, typename R>
	inline Or<L, R> OR(L l, R r) { return { l, r }; }

	template<typename P> inline Not<P> NOT(P p) { return { p }; }
}


// 使い方　例
// using namespace Engine::World;
// 
// // すべての Transform と Mesh を持つ
// auto ents1 = Query::ViewWhere(rw, Query::All<Transform, Mesh>{});
// 
// // Transform はあるが Physics は無い
// auto ents2 = Query::ViewWhere(rw,
// 	Query::AND(Query::All<Transform>{}, Query::NOT(Query::Any<Physics>{}))
// );
// 
// // Transform を持ち、( Mesh or Camera ) を持つ
// auto ents3 = Query::ViewWhere(rw,
// 	Query::AND(Query::All<Transform>{},
// 		Query::OR(Query::Any<Mesh>{}, Query::Any<Camera>{}))
// );