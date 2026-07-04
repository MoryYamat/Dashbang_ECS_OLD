// input mapping

#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

#include <optional>

#include "Game/Input/Public/InputActionComponent.hpp"

#include <iostream>

namespace Game::Input
{
	class InputMapping
	{
	public:

		void bindKey(int keyCode, InputAction action)
		{
			keyToAction[keyCode] = action;
			// std::cout << "[InputMapping.h(bindKey)]: key binded " << keyCode << "\n";
		}

		std::optional<InputAction> getAction(int keyCode) const
		{
			auto it = keyToAction.find(keyCode);
			if (it != keyToAction.end()) return it->second;
			return std::nullopt;
		}

	private:
		// mapping
		std::unordered_map<int, InputAction> keyToAction;
	};
}

