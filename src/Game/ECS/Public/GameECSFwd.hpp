#pragma once

#include <cstdint>

namespace Game::ECS::Component
{
	enum class Team : uint8_t;
	struct TeamComponent;
	struct LifetimeComponent;
}

namespace Game::ECS::Tags
{
	struct PlayerCharacterTag;
	struct MobTag;
	struct EnemyCharacterTag;
	struct NeutralNPCTag;
}