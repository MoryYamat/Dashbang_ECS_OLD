#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include <string>
#include <unordered_map>

namespace Game::Character::Animation::Movement
{

	bool InitMoveAnimProfile(Engine::WorldSystem::Core::WorldCtx& ctx);

	struct MovementAnimationProfile
	{
		std::string idle;
		std::string runFwd;
		std::string runBack;
		std::string runRight;
		std::string runLeft;
		std::string runFwdLeft;
		std::string runFwdRight;
		std::string runBackLeft;
		std::string runBackRight;
	};


	struct MovementAnimationProfileDatabase
	{
		std::unordered_map<std::string, MovementAnimationProfile> movements;

		const MovementAnimationProfile* FindMovement(const std::string& id) const
		{
			auto it = movements.find(id);
			return (it == movements.end()) ? nullptr : &it->second;
		}
	};

}