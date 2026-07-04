#pragma once


#include <typeindex>
#include <concepts>


namespace Game::Common::Logic::FSM::Query
{
	template<typename T>
	concept FSMStateComponent = requires(T t)
	{
		{ t.current } -> std::convertible_to<std::type_index>;
	};
}