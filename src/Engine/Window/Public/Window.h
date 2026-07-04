// for systems window manager

#pragma once

// #include <GLAD/glad.h>
// #include <GLFW/glfw3.h>

#include <string>

struct GLFWwindow;

namespace Engine::Window
{
	class Window
	{
	public:
		Window();
		~Window();

		bool Initialize(int width, int height, const std::string& title);
		void Shutdown();

		void UpdateViewport();
		void SetSize(int width, int height);
		float GetAspect() { return static_cast<float>(mWidth) / (mHeight); }



		GLFWwindow* GetGLFWWindow() const { return mWindow; }
		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }

		void CaptureMouse();
		void ReleaseMouse();

	private:

		GLFWwindow* mWindow = nullptr;
		int mWidth = 0;
		int mHeight = 0;

	};
}