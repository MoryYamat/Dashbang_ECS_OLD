#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"

// #include "Engine/FSM/Public/Core/Types.hpp"
#include <unordered_map>
#include <string>
#include <string_view>

namespace Game::Character::FSM::Movement
{

	Engine::FSM::Core::FieldResolver MakeMovementFieldResolver()
	{
		static const std::unordered_map<std::string, std::uint16_t> kTable =
		{
			{std::string(to_name(Field::MovementInputMag)), to_index(Field::MovementInputMag)},
		};

		return [](std::string_view name, std::uint16_t& out) -> bool
			{
				auto it = kTable.find(std::string(name));
				if (it == kTable.end()) return false;
				out = it->second;
				return true;
			};
	}
}