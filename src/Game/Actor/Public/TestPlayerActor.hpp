#pragma once

#include "Engine/Public/EngineFwd.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Actor
{

	class TestPlayerActor
	{
	public:
		TestPlayerActor(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader* shader);

		~TestPlayerActor() = default;

	private:

	};

	class TestPlayerCursorActor
	{
	public:
		TestPlayerCursorActor(Engine::WorldSystem::Core::WorldCtx& ctx);

	private:

	};
}