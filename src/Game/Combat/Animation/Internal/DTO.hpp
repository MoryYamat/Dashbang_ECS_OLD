// Skill名 に対する Animation を定義するために 作者が定義する DTO

#pragma once

#include <string>


namespace Game::Combat::Animation
{
	struct AnimDTO
	{
		std::string name;			// Animation name (!= skill name)
		std::string clip;			// "idle_default / " (clip名)
	};
}