#pragma once


// #include <glad/glad.h>
#include <GLM/glm.hpp>
// #include <GLFW/glfw3.h>

#include <unordered_map>


struct GLFWwindow;// 前方宣言

namespace Engine::Input
{

	struct RawInputState
	{
		std::unordered_map<int, bool> keyState;
		std::unordered_map<int, bool> previousKeyState;

		// マウスデータ
		glm::vec2 mousePosition = glm::vec2(0.0f);
		glm::vec2 mouseDelta = glm::vec2(0.0f);
		float scrollDelta = 0.0f;


		void ClearTransient() {
			scrollDelta = 0.0f;
			mouseDelta = glm::vec2(0.0f);
		}
	};

	class InputManager
	{
	public:

		InputManager(GLFWwindow* window);
		void Update();// 毎フレームRawInputStateを更新

		// Raw Input getter
		const RawInputState& GetRawInput() { return mRawInput; }
		RawInputState& GetRawInputMutable() { return mRawInput; }// 書き換え用


		// 将来的にMappingContextなども保持する

	private:
		// glfw window context
		GLFWwindow* mWindow;

		// raw input struct
		RawInputState mRawInput;

		// first mouse
		bool mFirstMouse;

		// 
		glm::vec2 mLastMousePosition;


		void updateMouseDelta();
		void updateKeyStates();
	};
}