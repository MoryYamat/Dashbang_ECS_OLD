//

#pragma once

#include "Engine/ECS/EntityManager.h"

//#include "Engine/Graphics/Private/Renderer/Shader.h"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Actor::Map
{
	class TestRockActor
	{
	public:
		TestRockActor(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Shader* shader);

	private:
	};
}
