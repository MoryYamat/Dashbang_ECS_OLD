#pragma once

#include <cassert>

namespace Engine 
{ 
	namespace ECS 
	{ 
		class EntityMgr; 
	} 
	namespace Graphics
	{
		namespace Render 
		{ 
			class Shader; 
			struct RenderContext; 
		}
	}
	namespace Input
	{
		class InputManager;
	}
	namespace Window
	{
		class Window;
	}
}

namespace Game::Common
{
	struct AppContext
	{
		Engine::ECS::EntityMgr* ecs = nullptr;
		Engine::Graphics::RenderContext* renderCtx = nullptr;

		Engine::Window::Window* window = nullptr;
		Engine::Input::InputManager* input = nullptr;

		Engine::Graphics::Render::Shader* shader = nullptr;

		// 安全アクセサ（必要な分だけ用意）
		Engine::ECS::EntityMgr& ECS() const { assert(ecs);        return *ecs; }
		Engine::Graphics::RenderContext& RC() const { assert(renderCtx);  return *renderCtx; }
		Engine::Window::Window& Window() const { assert(window);     return *window; }
		Engine::Input::InputManager& Input() const { assert(input);      return *input; }
		Engine::Graphics::Render::Shader& Shader() const { assert(shader); return *shader; }
	};
}