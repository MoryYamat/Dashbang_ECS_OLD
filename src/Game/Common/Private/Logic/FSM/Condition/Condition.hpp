// interface

#pragma once

namespace Game::Common::Logic::FSM::Condition
{
	// IConditionは、FSMの遷移条件を定義するインターフェース
	template<typename Context>
	struct ICondition
	{
		virtual ~ICondition() = default;
		virtual bool evaluate(const Context& ctx) const = 0;
	};
}