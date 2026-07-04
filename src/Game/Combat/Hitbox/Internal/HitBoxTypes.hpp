#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Physics/Public/Geometry2DTypes.hpp"

#include "Game/Combat/Skill/Public/CoreFwd.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"
#include "Game/Combat/HitBox/Internal/DTO.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>


#include <GLM/glm.hpp>

namespace Game::Combat::HitBox
{
	struct CanonicalHitBoxData
	{
		HitBoxID id;								// 定義順に0～採番
		Engine::Physics::ShapeKind shape = Engine::Physics::ShapeKind::None;			// 将来 enum に正規化
		float radius = 0.f;			// circle用
		float angle = 0.f;			// Sector 用 (rad //(DTO→Degree/ Canonical→Rad))
		float length = 0.f;			// Sector用
		glm::vec2 halfExtents = { 0.f, 0.f };	// Squeare/Obb
		glm::vec2 offset = glm::vec2(0.f);		// owner基準
	};

	struct HitBoxCatalog
	{
		std::vector<CanonicalHitBoxData> hitBoxes;


		std::unordered_map<std::string, HitBoxID> nameToId;
		HitBoxID FindByName(std::string_view s) const
		{
			auto it = nameToId.find(std::string(s));
			if (it == nameToId.end())
			{
				return kInvalidHitBoxID;
			}
			return it->second;
		}
	};


	// Buffers
	struct HitBoxSpawnRequest
	{
		Engine::ECS::Core::Entity owner;		// 
		HitBoxID hitbox;						// どの定義を使うか

		// メタ情報
		Game::Combat::Skill::SkillID skill;		// どのスキル由来か
		Engine::FSM::Core::StateID state;		// どのフェーズで発生したか

		glm::vec2 position = glm::vec2(0.0f);	// 発生位置
		glm::vec2 direction = glm::vec2(0.0f);	// 方向

		Engine::ECS::Core::Entity target;		// ロックオンターゲット(必要か検討)
		float triggerTime = 0.f;				// Skill 内 の timeoffset(必要か検討)
		float lifetime = 0.f;					// 寿命
	};


	// pubilc
	struct HitBoxSpawnRequestBuffer
	{
		std::vector<HitBoxSpawnRequest> reqs;
		void clear() { reqs.clear(); }
	};


	//// HITBOX & SKill Mapping
	//struct SkillHitBoxMapping
	//{
	//	 std::unordered_map<Game::Combat::Skill::SkillID, HitBoxID> skill2hitbox;

	//	HitBoxID Resolve(Game::Combat::Skill::SkillID s) const
	//	{
	//		auto it = skill2hitbox.find(s);
	//		if (it == skill2hitbox.end()) return kInvalidHitBoxID;
	//		return it->second;
	//	}

	//	void Set(Game::Combat::Skill::SkillID s, HitBoxID h)
	//	{
	//		skill2hitbox[s] = h;
	//	}
	//};

	struct HitBoxComponent
	{
		HitBoxID def;
		Engine::ECS::Core::Entity owner;

		Game::Combat::Skill::SkillID skill;
		Engine::FSM::Core::StateID state;
	};

	struct HitBoxSpawnSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(const float dt);
	};
}