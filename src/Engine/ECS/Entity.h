#pragma once

#include <cstdint>

// コンパイル時に決定　(4294967295)
constexpr uint32_t INVALID_ENTITY_ID = static_cast<uint32_t>(-1);

namespace Engine::ECS
{
	// 4 Byte
	struct Entity
	{
		uint32_t id;

		// 演算子オーバーロード
		bool operator == (const Entity& other) const
		{
			return id == other.id;
		}
		// overload
		bool operator != (const Entity& other) const
		{
			return id != other.id;
		}

		bool isValid() const { return id != INVALID_ENTITY_ID; }

		static const Entity INVALID;

		// 無効なEntityIDの定義
		static constexpr uint32_t INVALID_ENTITY_ID = static_cast<uint32_t>(-1);

	};

}

inline const Engine::ECS::Entity Engine::ECS::Entity::INVALID = { Entity::INVALID_ENTITY_ID };