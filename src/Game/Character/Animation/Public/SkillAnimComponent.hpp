#pragma once


#include "Game/Combat/Animation/Public/AnimationFwd.hpp"
#include <string>


namespace Game::Character::Animation::Skill
{
	struct SkillAnimDecisionComponent
	{
		std::string clipKey;
		bool loop = false;
		float playRate = 1.0f;
		bool valid = false;
	};

	struct SkillAnimRequestComponent
	{
		bool active = false;

		::Game::Combat::Animation::AnimID id;
		std::string clipKey;
		bool loop = false;
		float playRate = 1.0f;

		// section
		float startTime = 0.f;		// seconds
		float endTime = -1.0f;		// seconds
		bool loopWithinRange = false;

		// イベントを保持する
		float ttl = 0.0f;
		int priority = 100;
	};
}