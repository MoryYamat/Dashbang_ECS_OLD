#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/Core/WorldAccess.hpp"
#include <vector>


namespace Engine::WorldSystem::Query
{
	// {e | P(e) = true}
	// 述語 pred(w, e) が true の Entity を返す
	template<typename Pred>
	std::vector<Engine::ECS::Core::Entity> ViewWhere(const Core::WorldRead& rw, Pred pred)
	{
		std::vector<Engine::ECS::Core::Entity> out;
		rw.ForEachAlive([&](Engine::ECS::Core::Entity e)
			{
				if (pred(rw, e)) out.push_back(e);
			});

		return out;
	}
}