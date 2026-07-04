#include "Engine/InputManager/Public/Types.hpp"



#include <iostream>

 #include <glad/glad.h>
#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>




Engine::Input::InputManager::InputManager(GLFWwindow* window)
	: mWindow(window)
	, mFirstMouse(true)
	, mLastMousePosition(0.0f)
{
	// 初期マウス位置
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mLastMousePosition = glm::vec2(x, y);
	// mFirstMouse = false;
}

void Engine::Input::InputManager::Update()
{
	mRawInput.previousKeyState = mRawInput.keyState;
	mRawInput.mouseDelta = glm::vec2(0.0f);
	mRawInput.scrollDelta = 0.0f;

	updateKeyStates();
	updateMouseDelta();
}

void Engine::Input::InputManager::updateKeyStates()
{
	mRawInput.keyState.clear();

	for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
	{
		// state = GLFW_PRESS or GLFW_RELEASE
		int state = glfwGetKey(mWindow, key);

		// create and add 
		mRawInput.keyState[key] = (state == GLFW_PRESS || state == GLFW_REPEAT);
		// std::cout << "[InputManager(updateKeyStates)]: updates key states" << key << std::endl;
	}

	for (int button = GLFW_MOUSE_BUTTON_1; button < GLFW_MOUSE_BUTTON_LAST; ++button)
	{
		int state = glfwGetMouseButton(mWindow, button);
		mRawInput.keyState[button] = (state == GLFW_PRESS);
		//if (state == GLFW_PRESS)
		//{
		//	std::cout << "[InputManager(updateKeyStates]: mouse clicked " << button << ", " << state << std::endl;
		//}
	}
}

void Engine::Input::InputManager::updateMouseDelta()
{
	double xpos, ypos;
	glfwGetCursorPos(mWindow, &xpos, &ypos);
	glm::vec2 currentMousePos = glm::vec2(xpos, ypos);

	if (mFirstMouse)
	{
		mLastMousePosition = currentMousePos;
		mFirstMouse = false;
	}

	mRawInput.mouseDelta = currentMousePos - mLastMousePosition;
	mRawInput.mousePosition = currentMousePos;

	mLastMousePosition = currentMousePos;
}