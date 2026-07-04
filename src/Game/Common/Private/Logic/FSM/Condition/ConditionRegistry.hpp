#pragma once

#include "Game/Common/Private/Logic/FSM/Condition/Condition.hpp"
#include "Game/Common/Private/Logic/FSM/FSMRuntime.hpp"

#include <memory>
#include <unordered_map>

#include <stdexcept>

namespace Game::Common::Logic::FSM::Runtime
{
	// ConditionRegistryは、FSMの条件を管理するクラス
	template<typename Context>
	class ConditionRegistry
	{
	public:
		using ConditionPtr = std::shared_ptr<ICondition<Context>>;

		// 条件を登録するメソッド
		void registerCondition(std::string_view id, ConditionPtr condition)
		{
			conditions[id] = std::move(condition);
		}

		ConditionPtr lookup(std::string_view id) const
		{
			// 条件を検索し、見つかった場合はポインタを返す
			auto it = conditions.find(id);
			if(it != conditions.end())
			{
				return it->second;
			}
			throw std::runtime_error("Condition not found:" + std::string(id));// 例外を投げる
		}

	private:
		std::unordered_map<std::string_view, ConditionPtr> conditions;// 条件のマップ
	};

	// 使用例:
	// using namespace Game::Common::Logic::FSM::Runtime;
	// 
	// ConditionRegistry<MovementAxis> registry;
	// 
	// registry.registerCondition("can_move", std::make_shared<CanMoveCondition>());
	// registry.registerCondition("should_stop", std::make_shared<ShouldStopCondition>());
	// 
	// auto transitions = buildRuntimeTransitions<MovementFSM>(registry);
}