// Functions that provide auxiliary functions for ECS
// ECSの補助機能を提供する関数群
#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/ECS/Public/Entity.hpp"

#include <cstdint>
#include <string>

namespace Engine::ECS
{
	namespace EntityUtils
	{
		// For name resolution
		Engine::ECS::Entity getEntityByName(Engine::ECS::EntityMgr& ecs, const std::string& name);

		// 親と関連子Entityを削除予約にマークする
		void MarkForPendingDestroyWithChildren(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity parent, const std::vector<Engine::ECS::Entity>& children,
			int delayFrames = 0, bool fadeOut = false);

		// スキルインスタンスが子を持たない場合／オブジェクト単体を削除予約にマークする
		void MarkForPendingDestroy(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity entity, int delayFrames = 0, bool fadeOut = false);

		Engine::ECS::Core::Entity getEntityByName(Engine::WorldSystem::Core::WorldCtx& ctx, const std::string& name);
	}
}

