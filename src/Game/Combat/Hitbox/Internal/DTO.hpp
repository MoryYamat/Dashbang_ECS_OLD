// Skill名 に対する Hitbox(判定) を 定義するために作者が定義するDTO
// SkillRuntimeDTO と 統合するかは一考の余地あり
#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Game::Combat::HitBox
{
	struct HitBoxDTO
	{
		std::string name;		// hitbox name(!= skill name), 
		std::string shape;		// "Circle", "Sector",...
		float radius;			// circle用
		float angle;			// Sector 用 (degree)
		float length;			// Sector用
		glm::vec2 halfExtents;  // Square/Obb
		glm::vec2 offset;
	};
}

// testができたら std::string sync;を追加 