#pragma once

#include "Engine/WorldSystem/Private/Core/World.hpp"
#include "Engine/ECS/Public/ECSFwd.hpp"

#include <utility>

namespace Engine::WorldSystem::Core
{
	struct WorldRead
	{
		const World& w;

		template<typename T>
		[[nodiscard]] const T* TryGet(Engine::ECS::Core::Entity e) const;

		template<typename T>
		[[nodiscard]] bool Has(Engine::ECS::Core::Entity e) const;

		template<typename Fn>
		void ForEachAlive(Fn&& fn) const;

		// resource
		template<typename T>
		[[nodiscard]] const T* TryGetResource()const;

		template<typename T>
		[[nodiscard]] const T& GetResource() const;

		template<typename T>
		[[nodiscard]] bool HasResource() const;

	};

	struct WorldWrite
	{
		World& w;

		Engine::ECS::Core::Entity Create() { return w.Create();}

		void Destroy(Engine::ECS::Core::Entity e) {w.Destroy(e);}

		template<typename T, typename... A>
		T& Add(Engine::ECS::Core::Entity e, A&&... a);

		template<typename T>
		void Remove(Engine::ECS::Core::Entity e);

		template<typename T>
		[[nodiscard]] T& Get(Engine::ECS::Core::Entity e);

		template<typename T>
		[[nodiscard]] const T& Get(Engine::ECS::Core::Entity e) const;

		template<typename T>
		[[nodiscard]] const T* TryGetWithWarnOnce(Engine::ECS::Core::Entity e) const;

		template<typename T>
		[[nodiscard]] T* TryGetWithWarnOnce(Engine::ECS::Core::Entity e);

		//読み取り
		template<typename T>
		[[nodiscard]] bool Has(Engine::ECS::Core::Entity e) const;

		template<typename T>
		[[nodiscard]] T* TryGet(Engine::ECS::Core::Entity e);

		template<typename Fn>
		void ForEachAlive(Fn&& fn);

		// resource
		template<typename T, typename... A>
		T& CreateResource(A&&... a);

		template<typename T>
		[[nodiscard]] T* TryGetResource();

		template<typename T>
		[[nodiscard]] const T* TryGetResource()const;

		template<typename T>
		[[nodiscard]] T& GetResource();

		template<typename T>
		[[nodiscard]] const T& GetResource() const;

		template<typename T>
		[[nodiscard]] bool HasResource() const;

		template<typename T>
		void RemoveResource();

	};
}

#include "Engine/WorldSystem/Private/Core/WorldAccess.ipp"