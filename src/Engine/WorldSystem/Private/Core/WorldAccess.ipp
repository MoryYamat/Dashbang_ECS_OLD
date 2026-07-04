#pragma once

#include "Engine/WorldSystem/Private/Core/WorldAccess.hpp"
#include "Engine/ECS/Private/Core/Registry.hpp"

#include <utility>

namespace Engine::WorldSystem::Core
{
	template<typename T>
	[[nodiscard]] const T* WorldRead::TryGet(Engine::ECS::Core::Entity e) const
	{
		return w.Registry().TryGet<T>(e);
	}

	template<typename T>
	[[nodiscard]] bool WorldRead::Has(Engine::ECS::Core::Entity e) const
	{
		return w.Registry().HasComponent<T>(e);
	}

	template<typename Fn>
	void WorldRead::ForEachAlive(Fn&& fn) const
	{
		w.Registry().ForEachAlive(std::forward<Fn>(fn));
	}

	// resource
	template<typename T>
	[[nodiscard]] const T* WorldRead::TryGetResource()const
	{
		return w.TryGetResource<T>();
	}

	template<typename T>
	[[nodiscard]] const T& WorldRead::GetResource() const
	{
		return w.GetResource<T>();
	}

	template<typename T>
	[[nodiscard]] bool WorldRead::HasResource() const
	{
		return w.HasResource<T>();
	}


	template<typename T, typename... A>
	T& WorldWrite::Add(Engine::ECS::Core::Entity e, A&&... a)
	{
		return w.Registry().AddComponent<T>(e, std::forward<A>(a)...);
	}

	template<typename T>
	void WorldWrite::Remove(Engine::ECS::Core::Entity e)
	{
		w.Registry().RemoveComponent<T>(e);
	}

	template<typename T>
	[[nodiscard]] T& WorldWrite::Get(Engine::ECS::Core::Entity e)
	{
		return w.Registry().Get<T>(e);
	}

	template<typename T>
	[[nodiscard]] const T& WorldWrite::Get(Engine::ECS::Core::Entity e) const
	{
		return w.Registry().Get<T>(e);
	}

	//読み取り
	template<typename T>
	[[nodiscard]] bool WorldWrite::Has(Engine::ECS::Core::Entity e) const
	{
		return w.Registry().HasComponent<T>(e);
	}

	template<typename T>
	[[nodiscard]] T* WorldWrite::TryGet(Engine::ECS::Core::Entity e)
	{
		return w.Registry().TryGet<T>(e);
	}

	template<typename T>
	[[nodiscard]] T* WorldWrite::TryGetWithWarnOnce(Engine::ECS::Core::Entity e)
	{
		return w.Registry().TryGetWithWarnOnce<T>(e);
	}

	template<typename T>
	[[nodiscard]] const T* WorldWrite::TryGetWithWarnOnce(Engine::ECS::Core::Entity e) const 
	{
		return w.Registry().TryGetWithWarnOnce<T>(e);
	}

	template<typename Fn>
	void WorldWrite::ForEachAlive(Fn&& fn)
	{
		w.Registry().ForEachAlive(std::forward<Fn>(fn));
	}

	// resource

	template<typename T, typename... A>
	T& WorldWrite::CreateResource(A&&... a)
	{
		return w.CreateResource<T>(std::forward<A>(a)...);
	}

	template<typename T>
	[[nodiscard]] T* WorldWrite::TryGetResource()
	{
		return w.TryGetResource<T>();
	}

	template<typename T>
	[[nodiscard]] const T* WorldWrite::TryGetResource()const
	{
		return w.TryGetResource<T>();
	}

	template<typename T>
	[[nodiscard]] T& WorldWrite::GetResource()
	{
		return w.GetResource<T>();
	}

	template<typename T>
	[[nodiscard]] const T& WorldWrite::GetResource() const
	{
		return w.GetResource<T>();
	}

	template<typename T>
	[[nodiscard]] bool WorldWrite::HasResource() const
	{
		return w.HasResource<T>();
	}

	template<typename T>
	void WorldWrite::RemoveResource()
	{
		w.RemoveResource<T>();
	}
}