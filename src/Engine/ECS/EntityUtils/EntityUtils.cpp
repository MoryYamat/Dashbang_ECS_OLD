#include "EntityUtils.h"

#include "Engine/ECS/Component/Utils/NameComponent.h"
#include "Engine/ECS/Component/Tags/PendingDestroyComponent.h"
#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Engine/Component/Private/Utils/NameComponent.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <iostream>


namespace Engine::ECS::EntityUtils
{
	Engine::ECS::Core::Entity getEntityByName(Engine::WorldSystem::Core::WorldCtx& ctx, const std::string& name)
	{
		auto ents = Engine::WorldSystem::Query::ViewWhere(ctx.rw, Engine::WorldSystem::Query::All<Engine::Component::NameComponent>{});

		for (auto e : ents)
		{
			const auto& nameComp = *ctx.rw.TryGet<Engine::Component::NameComponent>(e);
			if (nameComp.name == name)
			{
				return e;
			}
		}
	}
}

Engine::ECS::Entity Engine::ECS::EntityUtils::getEntityByName(EntityMgr& ecs, const std::string& name)
{
	namespace Ops = Engine::ECS::Ops;
	// Efficiency Issues
	// Efficiency Issues
	// Efficiency Issues
	// Efficiency Issues
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Utils::NameComponent>())
	{
		const Engine::ECS::Component::Utils::NameComponent& nameComp = Ops::Get<Engine::ECS::Component::Utils::NameComponent>(ecs, e);
		if (nameComp.name == name)
		{
			//std::cout << "[EntityUtils.cpp]Entity ID: " << e.id << " Entity Name: " << name << std::endl;
			return e;
		}
	}

	std::cerr << "[EntityUtils.cpp]: There is no entity named: [[ "<< name << " ]] ." << std::endl;

	return Engine::ECS::Entity::INVALID;
}

void Engine::ECS::EntityUtils::MarkForPendingDestroyWithChildren(EntityMgr& ecs, Engine::ECS::Entity parent, const std::vector<Engine::ECS::Entity>& children,
	int delayFrames, bool fadeOut)
{
	ecs.addComponent<Engine::ECS::Component::Tags::PendingDestroyComponent>(parent, Engine::ECS::Component::Tags::PendingDestroyComponent{ delayFrames, fadeOut });
	for (Engine::ECS::Entity child : children)
	{
		if (ecs.isAlive(child))
		{
			ecs.addComponent<Engine::ECS::Component::Tags::PendingDestroyComponent>(child, Engine::ECS::Component::Tags::PendingDestroyComponent{ delayFrames, fadeOut });
		}
	}
}

void Engine::ECS::EntityUtils::MarkForPendingDestroy(EntityMgr& ecs, Engine::ECS::Entity entity, int delayFrames, bool fadeOut)
{
	if (ecs.isAlive(entity))
	{
		ecs.addComponent<Engine::ECS::Component::Tags::PendingDestroyComponent>(entity, Engine::ECS::Component::Tags::PendingDestroyComponent{ delayFrames, fadeOut });
	}
}