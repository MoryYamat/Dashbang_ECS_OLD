#include "GlobalCleanupSystem.h"

#include "Engine/ECS/Component/Graphics/MeshComponent.h"

#include "Engine/ECS/Component/Tags/PendingDestroyComponent.h"

void Engine::ECS::GrobalSystem::RunCleanup(Engine::ECS::EntityMgr& ecs)
{
	for (Entity e : ecs.view<Engine::ECS::Component::Tags::PendingDestroyComponent>())
	{
		auto& destroy = ecs.get<Engine::ECS::Component::Tags::PendingDestroyComponent>(e);

		// 削除遅延フレーム消費
		if (destroy.delayFrames > 0)
		{
			destroy.delayFrames--;
			continue;
		}

		if (destroy.fadeOutEffect)
		{
			// フェードアウト演出処理...
		}

		ecs.destroyEntity(e);
	}
}