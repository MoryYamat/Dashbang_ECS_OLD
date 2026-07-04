// コンポーネント初期化の汎用テンプレート
// Generic template for component initialization

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/Window/Public/WindowFwd.hpp"


namespace Engine::ECS::Meta::Init
{
	template<typename Component>
	struct InitSystem
	{
		static void Init(Component& comp, Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity owner, const Engine::Window::Window& window)
		{
			// The default is to do nothing.
		}
	};
}

