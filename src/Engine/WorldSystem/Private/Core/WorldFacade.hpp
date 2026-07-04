#pragma once

#include "Engine/WorldSystem/Private/Core/World.hpp"
#include "Engine/WorldSystem/Private/Core/WorldAccess.hpp"

// 未使用
// Systemでは、WorldCtxを引数として扱うことにする
namespace Engine::WorldSystem::Core
{
	class WorldFacade
	{
	public:

		explicit WorldFacade(World& w) : world_(w){}

		World& Get() { return world_; }
		const World& Get() const { return world_; }

		WorldRead Read() const { return { world_ }; }
		WorldWrite Write() { return { world_ }; }

	private:
		World& world_;
	};
}