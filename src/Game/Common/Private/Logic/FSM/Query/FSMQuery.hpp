#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "FSMQuery_Concepts.hpp"

#include <optional>
#include <typeindex>

// World仕様へ変更必要

namespace Game::Common::Logic::FSM::Query
{

	// EntityのFSMの現在の状態を観測・照会するQuery
	class FSMQuery
	{
	private:
		Engine::ECS::Entity entity;
		Engine::ECS::EntityMgr& ecs;

	public:

		FSMQuery(const Engine::ECS::Entity e, Engine::ECS::EntityMgr& ecs)
			: entity(e), ecs(ecs) { }

		template<FSMStateComponent StateComponent>
		bool isInState(std::type_index tag) const
		{
			if (!ecs.hasComponent<StateComponent>(entity)) return false;
			const auto& comp = ecs.get<StateComponent>(entity);
			return comp.current == tag;
		}

		template<FSMStateComponent StateComponent, typename Tag>
		bool isInState() const
		{
			return isInState<StateComponent>(typeid(Tag));
		}

		template<FSMStateComponent StateComponent>
		std::optional<std::type_index> getCurrentState() const 
		{
			if (!ecs.hasComponent<StateComponent>(entity)) return std::nullopt;
			const auto& comp = ecs.get<StateComponent>(entity);
			return comp.current;
		}

		template<FSMStateComponent StateComponent, typename Tag>
		bool isNotInState() const
		{
			return !isInState<StateComponent, Tag>();
		}

		template<FSMStateComponent StateComponent>
		bool isInAnyOf(std::initializer_list<std::type_index> tags) const {
			if (!ecs.hasComponent<StateComponent>(entity)) return false;
			const auto& comp = ecs.get<StateComponent>(entity);
			for (auto& tag : tags) {
				if (comp.current == tag) return true;
			}
			return false;
		}

		// inAnyof / notInState etc.

		template<FSMStateComponent StateComponent>
		std::optional<std::type_index>  getPreviousState() const
		{
			if (!ecs.hasComponent<StateComponent>(entity)) return std::nullopt;
			const auto& comp = ecs.get<StateComponent>(entity);
			return comp.previous;
		}

		template<FSMStateComponent StateComponent>
		bool entered(std::type_index s) const
		{
			auto p = getPreviousState<StateComponent>();
			auto c = getCurrentState<StateComponent>();

			return p && c && *p != s && *c == s;
		}
	};
}