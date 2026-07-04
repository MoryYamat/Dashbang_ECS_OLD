#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"
#include "Engine/FSM/Public/FSMApi.hpp"

// #include "Engine/FSM/Public/Core/Types.hpp"
#include <unordered_map>
#include <string>
#include <string_view>

namespace Game::Character::FSM::Skill
{
	Engine::FSM::Core::FieldResolver MakeSkillFieldResolver()
	{
		static const std::unordered_map<std::string, std::uint16_t> kTable =
		{
			// {std::string(to_name(Field::ElapsedTime)), to_index(Field::ElapsedTime)},
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