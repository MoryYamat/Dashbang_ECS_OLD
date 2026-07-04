// Team Component

#pragma once
#include <cstdint>

namespace Game::ECS::Component
{
	// 
	enum class Team : uint8_t 
	{
		Neutral = 0,
		PlayerTeam = 1,
		EnemyTeam = 2
	};

	// 
	struct TeamComponent
	{
		Team team;
	};


}