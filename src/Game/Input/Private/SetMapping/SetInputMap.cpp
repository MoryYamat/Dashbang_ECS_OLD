

#include "Game/Input/Private/InputMapping.h"

#include "Game/Input/Public/InputActionComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <iostream>

namespace Game::Input
{
	static void SetInputMapping(InputMapping& map)
	{
		
		map.bindKey(GLFW_KEY_W, Game::Input::InputAction::MoveForward);
		map.bindKey(GLFW_KEY_S, Game::Input::InputAction::MoveBackward);
		map.bindKey(GLFW_KEY_D, Game::Input::InputAction::MoveRight);
		map.bindKey(GLFW_KEY_A, Game::Input::InputAction::MoveLeft);
		map.bindKey(GLFW_MOUSE_BUTTON_1, Game::Input::InputAction::CastSkill1);
		map.bindKey(GLFW_MOUSE_BUTTON_2, Game::Input::InputAction::CastSkill2);
		map.bindKey(GLFW_KEY_1, Game::Input::InputAction::CastSkill3);
		map.bindKey(GLFW_KEY_2, Game::Input::InputAction::CastSkill4);

		std::cout << "[Init Input]: Input map initialization successful\n";
	}

	void InitAllInputSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& map = ctx.ww.CreateResource<InputMapping>();

		SetInputMapping(map);
	}
}
