// 
#pragma once

#include "Engine/ECS/Meta/InitComponent/ApplyInitForComponent.h"

#include "Engine/ECS/EntityManager.h"


namespace Engine::ECS::Meta::Init
{
	template<typename... Components>
	void ApplyAllDeferredInitializations(Engine::ECS::EntityMgr& ecs, Engine::Window::Window& window)
	{
		(ApplyDeferredInitialization<Components>(ecs, window), ...);
	}
}
