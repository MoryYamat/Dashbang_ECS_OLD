#include "Engine/Window/Public/Window.h"

#include <glad/glad.h>// 順序
#include <GLFW/glfw3.h>

#include <iostream>

Engine::Window::Window::Window()
{}

Engine::Window::Window::~Window() {
	Shutdown();
}

bool Engine::Window::Window::Initialize(int width, int height, const std::string& title)
{
	if (!glfwInit())
	{
		std::cerr << "[Window.cpp]: Failed to initialize GLFW\n";
		return false;
	}

	mWidth = width;
	mHeight = height;

	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		std::cerr << "[Window.cpp]: Failed to create window\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[WindowManager.cpp (Initialize)]: Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetWindowUserPointer(mWindow, this);

	glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
		auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (self)  self->SetSize(width, height);
		});

	UpdateViewport();

	return true;
}

void Engine::Window::Window::Shutdown()
{
	if (mWindow)
	{
		glfwDestroyWindow(mWindow);
		mWindow = nullptr;
	}
}

void Engine::Window::Window::UpdateViewport()
{
	glViewport(0, 0, mWidth, mHeight);
}

void Engine::Window::Window::SetSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
	UpdateViewport();
}

void Engine::Window::Window::CaptureMouse()
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Window::Window::ReleaseMouse()
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}