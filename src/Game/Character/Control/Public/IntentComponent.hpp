#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Game/Combat/Skill/Public/CoreFwd.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace Game::Character::Control
{
	struct FacingIntentComponent
	{
		glm::vec2 front = glm::vec2(0.0f, 1.0f); // 正面の方向
		bool isValid = false; //入力が有効かどうか
	};

	struct MovingIntentComponent
	{
		glm::vec2 direction = glm::vec2(0.0f);	// ワールド空間における方向// directionは使用側で正規化する
		bool isActive = false;					// 意図としての移動の有無
	};


	struct SkillIntentComponent
	{
		std::vector<Game::Combat::Skill::SkillRequest> requests;
	};

}