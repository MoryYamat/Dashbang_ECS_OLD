#pragma once

#include "Game/Common/Private/Logic/FSM/Condition/Condition.hpp"
#include <typeindex>
#include <memory>
#include <tuple>
#include <vector>

namespace Game::Common::Logic::FSM::Runtime
{
	template<typename Context>
	struct RuntimeContext;// Axisに基づく実行時コンテキスト
	// 使用側
	// template<typename Axis>
	// struct RuntimeContext<Axis>{...}と定義，毎フレーム

	struct RuntimeTransition
	{
		std::type_index from; // 遷移元の状態の型
		std::type_index to;   // 遷移先の状態の型
		std::shared_ptr<ICondition<RuntimeContext>> condition; // 遷移条件のインスタンス
	};

	template<typename FSMDef>
	std::vector<RuntimeTransition> buildRuntimeTransitions(const ConditionRegistry& registry)
	{
		std::vector<RuntimeTransition> transitions;

		std::apply([&](auto... trans)
			{
				(..., transitions.push_back(RuntimeTransition{
					typeid(typename decltype(trans)::From),
					typeid(typename decltype(trans)::To),
					registry.lookup(decltype(trans)::conditionId)
					}));
			}, typename FSMDef::TransitionSets::Types{});

		return transitions;
	}
}