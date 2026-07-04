// conditions that are composed of other conditions

#pragma once

#include "Game/Common/Logic/Condition/Condition.hpp"
#include <memory>
#include <vector>

namespace Game::Common::Logic::FSM::Condition
{
	// AND条件を評価するクラス
	template<typename Context>
	struct AndCondition : ICondiition<Context>
	{
		std::vector<std::shared_ptr<ICondition<Context>>> children;

		explicit AndCondition(std::vector<std::shared_ptr<ICondition<Context>>> conds)
			: children(std::move(conds)) {}

		bool evaluate(const Context& ctx) const override
		{
			for (const auto& cond : children)
			{
				if (!cond->evaluate(ctx))
					return false; // いずれかの条件がfalseなら全体もfalse
			}
			return true; // 全ての条件がtrueなら全体もtrue
		}
	};

	// OR条件を評価するクラス
	template<typename Context>
	struct OrCondition : ICondition<Context>
	{
		std::vector<std::shared_ptr<ICondition<Context>>> children;

		explicit OrCondition(std::vector<std::shared_ptr<ICondition<Context>>> conds)
			: children(std::move(conds)) {}

		bool evaluate(const Context& ctx) const override
		{
			for (const auto& cond : children)
			{
				if (cond->evaluate(ctx))
					return true; // いずれかの条件がtrueなら全体もtrue
			}
			return false; // 全ての条件がfalseなら全体もfalse
		}
	};

	// NOT条件を評価するクラス
	template<typename Context>
	struct NotCondition : ICondition<Context>
	{
		std::shared_ptr<ICondition<Context>> child;

		explicit NotCondition(std::shared_ptr<ICondition<Context>> cond)
			: child(std::move(cond)) {}


		bool evaluate(const RuntimeContext& ctx) const override
		{
			return !child->evaluate(ctx); // 子条件の評価結果を反転
		}
	};
}