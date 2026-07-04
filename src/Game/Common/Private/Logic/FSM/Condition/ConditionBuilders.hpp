// ビルダーヘルパー関数群

#pragma once

#include "Game/Common/Private/Logic/FSM/Condition/Condition.hpp"
#include "Game/Common/Private/Logic/FSM/Condition/PrimitiveCondition.hpp"
#include "Game/Common/Private/Logic/FSM/Condition/ConstantConditions.hpp"
#include "Game/Common/Private/Logic/FSM/Condition/CompositeCondition.hpp"

#include <memory>

// 使用例：
// using namespace Game::Common::Logic::FSM::Condition;
// 
// auto cond = make_and<MyContext>({
// 	make_timer<MyContext>(2.0f),
// 	make_compare<int, MyContext>(
// 		[](const MyContext& ctx) { return ctx.hp; },
// 		CompareOperator::GreaterEqual,
// 		[](const MyContext&) { return 100; }
// 	)
// 	});

namespace Game::Common::Logic::FSM::Condition
{
	// 比較条件を構築するためのヘルパー関数
	template<typename T, typename Context>
	std::shared_ptr<ICondition<Context>> make_compare(
		std::function<T(const Context&)> lhs,
		std::function<T(const Context&)> rhs,
		CompareOperator op
	)
	{
		return std::make_shared<CompareCondition<T, Context>>(
			std::move(lhs),
			std::move(rhs),
			op
		);
	}

	// timer条件を構築するためのヘルパー関数
	template<typename Context>
	std::shared_ptr<ICondition<Context>> make_timer(float threshold)
	{
		return std::make_shared<TimerCondition<Context>>(threshold);
	}

	// true条件を構築するためのヘルパー関数
	template<typename Context>
	std::shared_ptr<ICondition<Context>> make_true()
	{
		return std::make_shared<TrueCondition<Context>>();
	}

	// false条件を構築するためのヘルパー関数
	template<typename Context>
	std::shared_ptr<ICondition<Context>> make_false()
	{
		return std::make_shared<FalseCondition<Context>>();
	}

	// AND条件を構築するためのヘルパー関数
	template<typename Context>
	std::shared_ptr<ICondition<Context>> make_and(
		std::vector<std::shared_ptr<ICondition<Context>>> children )
	{
		return std::make_shared<AndCondition<Context>>(std::move(children));
	}

	// OR条件を構築するためのヘルパー関数
	template<typename Context>
	std::shared_ptr<ICondition<Context>> make_or(
		std::vector<std::shared_ptr<ICondition<Context>>> children) {
		return std::make_shared<OrCondition<Context>>(std::move(children));
	}

	// NOT条件を構築するためのヘルパー関数
	template<typename Context>
	std::shared_ptr<ICondition<Context>> make_not(
		std::shared_ptr<ICondition<Context>> child) {
		return std::make_shared<NotCondition<Context>>(std::move(child));
	}

	// 使用例:
	// using namespace Game::Common::Logic::FSM::Condition;
	// auto cond = make_and<MyContext>({
	// 	make_timer<MyContext>(2.0f),
	// 	make_compare<int, MyContext>(
	// 		[](const MyContext& ctx) { return ctx.hp; },
	// 		CompareOperator::GreaterEqual,
	// 		[](const MyContext&) { return 50; }
	// 	)
	// 	});
}