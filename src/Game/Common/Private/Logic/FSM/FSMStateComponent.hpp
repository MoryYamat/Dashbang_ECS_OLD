#pragma once

#include <typeindex>

namespace Game::Common::Logic::FSM
{
	// FSMの状態を保持するコンポーネント
	template<typename Axis>
	struct FSMStateComponent
	{
		std::type_index current; // 現在の状態を表す型のインデックス

		FSMStateComponent() : current(typeid(void)) {} // 初期状態はvoid型

		explicit FSMStateComponent(std::type_index init)
			: current(init) {} // 初期状態を指定して初期化
	};
}