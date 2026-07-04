//

#pragma once

#include "Game/Common/Private/Logic/FSM/FSMRuntime.hpp"

#include <utility>
#include <variant>
#include <functional>

namespace Game::Common::Logic::FSM::Condition
{


	// プリミティブな比較演算子を定義する列挙型
	enum class CompareOperator
	{
		Equal,		// 等しい
		NotEqual, // 等しくない
		Less, 		// より小さい
		LessEqual, // 以下
		Greater, 	// より大きい
		GreaterEqual // 以上
	};



	// プリミティブな比較条件を表すテンプレートクラス
	template<typename T, typename Context>
	struct CompareCondition : ICondition
	{
		std::function<T(const Context&)> lhs; // 左辺の値を取得する関数
		std::function<T(const Context&)> rhs; // 右辺の値を取得する関数
		CompareOperator op; // 比較演算子

		CompareCondition(
			std::function<T(const Context&)> l,
			std::function<T(const Context&)> r,
			CompareOperator op)
			: lhs(std::move(l)), rhs(std::move(r)), op(op)) {}

		bool evaluate(const Context& ctx) const override
		{
			T l = lhs(ctx); // 左辺の値を取得
			T r = rhs(ctx); // 右辺の値を取得

			switch (op)
			{
			case CompareOperator::Equal: return l == r; // 等しい
			case CompareOperator::NotEqual: return l != r; // 等しくない
			case CompareOperator::Less: return l < r; // より小さい
			case CompareOperator::LessEqual: return l <= r; // 以下
			case CompareOperator::Greater: return l > r; // より大きい
			case CompareOperator::GreaterEqual: return l >= r; // 以上
			}
			return false; // デフォルトはfalse（未定義の演算子の場合）
		}
	};


	// 使用例:
	// using Ctx = MyGameRuntimeContext;
	// auto cond = std::make_shared<CompareCondition<float, Ctx>>(
	// 	[](const Ctx& ctx) { return ctx.getHP(); },
	// 	CompareOperator::LessEqual,
	// 	[](const Ctx&) { return 0.3f; }
	// );


	// TimerConditionは、特定の時間が経過したかどうかを評価する条件
	template<typename Context>
	struct TimerCondition : ICondition<Context>
	{
		float threshold; // タイマーの閾値（秒単位）

		TimeCondition(float time) : threshold(time) {}

		bool evaluate(const Context& ctx) const override
		{
			return ctx.getElapsedTime() >= threshold; // ctxから経過時間を取得し、閾値と比較
		}
	};
}