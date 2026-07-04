// 一括初期化のための関数テンプレート
// Function template for bulk initialization


#pragma once 

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Meta/InitPhase.h"
#include "Engine/ECS/Meta/InitPhaseSpecialization.h"
#include "Engine/ECS/Meta/InitComponent/InitSystem.h"


namespace Engine::ECS::Meta::Init
{
	template<typename Component>
	void ApplyDeferredInitialization(Engine::ECS::EntityMgr& ecs, Engine::Window::Window& window)
	{
		constexpr InitPhase phase = GetInitPhase<Component>();

		if constexpr (phase == InitPhase::Deferred)
		{
			///std::cout << "[Init] Deferred Init for: " << typeid(Component).name() << std::endl;

			auto entities = ecs.view<Component>();
			//std::cout << " - Found " << entities.size() << " entities." << std::endl;

			for (Entity e : ecs.view<Component>())
			{
				auto& comp = ecs.get<Component>(e);
				InitSystem<Component>::Init(comp, ecs, e, window);
			}
		}
	}
}

