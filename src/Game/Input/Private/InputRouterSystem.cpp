#include "Game/Input/Public/InputApi.hpp"

#include "Game/Input/Private/InputMapping.h"
#include "Game/Input/Public/InputActionComponent.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/InputManager/Public/Types.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <cassert>
#include <iostream>

namespace Game::Input
{
	void InputRouterSystem(Engine::WorldSystem::Core::WorldCtx& ctx, const Engine::Input::RawInputState& raw)
	{
		auto& map = ctx.ww.GetResource<Game::Input::InputMapping>();

		auto ents = Engine::WorldSystem::Query::ViewWhere(ctx.rw,
			Engine::WorldSystem::Query::All<Game::Input::InputActionComponent>{});

		for (const auto& e : ents)
		{
			auto& input = ctx.ww.Get<Game::Input::InputActionComponent>(e);

			input.previous = input.current;
			input.current.clear();

			for (const auto& [key, isDown] : raw.keyState)
			{
				if (!isDown) continue;

				if (auto actionOpt = map.getAction(key))
				{
					input.current[*actionOpt] = true;
				}
			}
		}
	}
}
