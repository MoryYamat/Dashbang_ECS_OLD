#pragma once


#include "Engine/WorldSystem/Private/Core/Resource.hpp"

namespace Engine::WorldSystem::Core
{
	template<typename T, typename... A>
	T& World::CreateResource(A&&... a)
	{
		return Resources().Create<T>(std::forward<A>(a)...);
	}

	template<typename T>
	T* World::TryGetResource()
	{
		return Resources().TryGet<T>();
	}

	template<typename T>
	const T* World::TryGetResource() const 
	{
		return Resources().TryGet<T>();
	}

	template<typename T>
	T& World::GetResource()
	{
		return Resources().Get<T>();
	}

	template<typename T>
	const T& World::GetResource() const
	{
		return Resources().Get<T>();
	}

	template<typename T>
	bool World::HasResource() const
	{
		return Resources().Has<T>();
	}

	template<typename T>
	void World::RemoveResource()
	{
		Resources().Remove<T>();
	}
}