#pragma once

#include "Engine/WorldSystem/Private/Traits/ComponentTraits.hpp"
#include "Engine/ECS/Public/ECSFwd.hpp"

#include <memory>

namespace Engine::WorldSystem::Core { class Resource; }

namespace Engine::WorldSystem::Core
{
	class World
	{
	private:
		class Impl;
		std::unique_ptr<Impl> p_;

		Resource& Resources();
		const Resource& Resources() const;

	public:
		World();
		~World();

		Engine::ECS::Core::Entity Create();
		void Destroy(Engine::ECS::Core::Entity e);
		bool IsAlive(Engine::ECS::Core::Entity e) const;

		// registry参照
		Engine::ECS::Core::Registry& Registry();
		const Engine::ECS::Core::Registry& Registry() const;


		// Resource
		template<typename T, typename... A> T& CreateResource(A&&... a);
		template<typename T>				T* TryGetResource();
		template<typename T>		  const T* TryGetResource() const;
		template<typename T>				T& GetResource();
		template<typename T>		  const T& GetResource() const;
		template<typename T>			  bool HasResource() const;
		template<typename T>			  void RemoveResource();
	};
}

#include "Engine/WorldSystem/Private/Core/World.ipp"


//World /
// Core /
//	Entity.h
//	World.h
//	World.cpp
//	WorldFwd.h
// Query /
//	View.h
//	Filter.h
// Traits /
//	ComponentTraits.h
// Resource /
//	Resources.h
