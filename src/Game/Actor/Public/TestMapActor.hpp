#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Actor
{
	class TestTerrainMesh
	{
	public:
		TestTerrainMesh(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader* shader);
	private:
	};
}