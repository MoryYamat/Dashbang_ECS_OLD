#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/Log/Public/LogAPi.hpp"

#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <utility>
#include <cassert>

namespace Engine::ECS::Core
{
	class Registry
	{
	private:
		std::uint32_t nextId_ = 1;
		std::unordered_set<std::uint32_t> alive_;
		std::unordered_map<std::type_index,
			std::unordered_map<std::uint32_t, std::shared_ptr<void>>> pools_;

		mutable std::unordered_map<std::type_index,
			std::unordered_set<std::uint32_t>> warnedOnce_;

	public:
		// Entity
		Entity Create()
		{
			Entity e{ nextId_++ };
			alive_.insert(e.id);
			return e;
		}

		void Destroy(Entity e)
		{
			alive_.erase(e.id);
			for (auto& [t, pool] : pools_) pool.erase(e.id);
		}

		bool IsAlive(Entity e) const { return alive_.count(e.id) != 0; }

		// component
		template<typename T, typename... A>
		T& AddComponent(Entity e ,A&&... a)
		{
			assert(IsAlive(e));
			auto& bucket = pools_[typeid(T)];
			auto it = bucket.find(e.id);
			if (it != bucket.end())
			{
				Engine::Log::Write(Engine::Log::Level::Fatal, "Registry", typeid(T).name() + std::string("already exists"));
				assert(false && "AddComponent<T>: already exists");
			}
			auto sp = std::make_shared<T>(std::forward<A>(a)...);
			auto& ref = *sp;
			bucket.emplace(e.id, std::move(sp));
			return ref;
		}

		template<typename T>
		[[nodiscard]]T* TryGet(Entity e)
		{
			auto it = pools_.find(typeid(T));
			if (it == pools_.end()) return nullptr;
			auto jt = it->second.find(e.id);
			if (jt == it->second.end()) return nullptr;
			return static_cast<T*>(jt->second.get());
		}

		template<typename T>
		[[nodiscard]] const T* TryGet(Entity e) const
		{
			auto it = pools_.find(typeid(T));
			if (it == pools_.end()) return nullptr;
			auto jt = it->second.find(e.id);
			if (jt == it->second.end()) return nullptr;
			return static_cast<const T*>(jt->second.get());
		}

		template<typename T>
		[[nodiscard]] T& Get(Entity e)
		{
			auto* p = TryGet<T>(e);
			if (!p)
			{
				Log::Write(Engine::Log::Level::Fatal,
					"Registry", std::string("Registry missing: ") + typeid(T).name());
				assert(false && "Get<T>: component missing");
			}
			return *p;
		}

		template<typename T>
		[[nodiscard]] const T& Get(Entity e) const
		{
			auto* p = TryGet<T>(e);
			if (!p)
			{
				Log::Write(Engine::Log::Level::Fatal,
					"Registry", std::string("Registry missing: ") + typeid(T).name());
				assert(false && "Get<T>: component missing");
			}
			return *p;
		}

		template<typename T>
		[[nodiscard]] const T* TryGetWithWarnOnce(Entity e) const
		{
			const auto* p = TryGet<T>(e);
			if (p) return p;

			auto& warnedSet = warnedOnce_[typeid(T)];
			if (!warnedSet.contains(e.id))
			{
				warnedSet.insert(e.id);
				std::printf("[Warn] Entity %u missing component %s\n", e.id, typeid(T).name());
			}

			return nullptr;
		}

		template<typename T>
		[[nodiscard]] T* TryGetWithWarnOnce(Entity e)
		{
			auto* p = TryGet<T>(e);
			if (p) return p;

			auto& warnedSet = warnedOnce_[typeid(T)];
			if (!warnedSet.contains(e.id))
			{
				warnedSet.insert(e.id);
				std::printf("[Warn] Entity %u missing component %s\n", e.id, typeid(T).name());
			}

			return nullptr;
		}

		template<typename T>
		[[nodiscard]] bool HasComponent(Entity e) const 
		{
			auto it = pools_.find(typeid(T));
			return it != pools_.end() && it->second.count(e.id) != 0;
		}

		template<typename T>
		void RemoveComponent(Entity e)
		{
			auto it = pools_.find(typeid(T));
			if (it == pools_.end())return;
			auto& bucket = it->second;
			if (auto jt = bucket.find(e.id); jt != bucket.end()) bucket.erase(jt);
			if (bucket.empty()) pools_.erase(it);
		}

		template<typename Fn>
		void ForEachAlive(Fn&& fn) const
		{
			for (auto id : alive_) fn(Entity{ id });
		}
	};
}