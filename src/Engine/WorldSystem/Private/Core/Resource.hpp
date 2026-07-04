#pragma once

#include "Engine/ECS/Public/Entity.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <typeindex>
#include <unordered_map>
#include <memory>
#include <utility>
#include <iostream>
#include <cassert>

namespace Engine::WorldSystem::Core
{
	class Resource
	{
		std::unordered_map<std::type_index, std::shared_ptr<void>> store_;

	public:
		template<typename T, typename... A>
		T& Create(A&&... a)
		{
			auto& slot = store_[typeid(T)];
			if (!slot) slot = std::make_shared<T>(std::forward<A>(a)...);
			return *static_cast<T*>(slot.get());
		}

		template<typename T>
		T* TryGet()
		{
			auto it = store_.find(typeid(T));
			return (it == store_.end()) ? nullptr : static_cast<T*>(it->second.get());
		}

		template<typename T>
		const T* TryGet() const 
		{
			auto it = store_.find(typeid(T));
			return (it == store_.end()) ? nullptr : static_cast<const T*>(it->second.get());
		}

		template<typename T>
		[[nodiscard]]bool Has() const
		{
			return store_.count(typeid(T)) != 0;
		}

		template<typename T>
		T& Get()
		{
			auto* p = TryGet<T>();
			if (!p)
			{
				// std::fprintf(stderr, "Fatal: Resource <%s>\n", typeid(T).name());// グレーゾーン
				Log::Write(Engine::Log::Level::Fatal, 
					"Resource", std::string("Resource missing: ") + typeid(T).name());

				assert(false && "Resource missing");
			}
			return *p;
		}

		template<typename T>
		const T& Get() const
		{
			auto* p = TryGet<T>();
			if (!p)
			{
				// std::fprintf(stderr, "Fatal: Resource <%s>\n", typeid(T).name());// グレーゾーン
				Log::Write(Engine::Log::Level::Fatal, 
					"Resource", std::string("Resource missing: ") + typeid(T).name());

				assert(false && "Resource missing");
			}
			return *p;
		}

		template<typename T>
		void Remove()
		{
			store_.erase(typeid(T));
		}
	};
}