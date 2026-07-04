#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Combat/Skill/Public/CoreFwd.hpp"

#include "Game/Combat/Animation/Public/AnimationAPI.hpp"
#include "Game/Combat/Animation/Public/AnimationFwd.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace Game::Combat::Animation
{
	struct CanonicalAnimData
	{
		AnimID id;
		std::string clip;
	};

	struct AnimCatalog
	{
		std::vector<CanonicalAnimData> anims;

		std::unordered_map<std::string, AnimID> nameToId;
		AnimID FindByName(std::string_view s) const
		{
			auto it = nameToId.find(std::string(s));
			if (it == nameToId.end())
			{
				return kInvalidAnimID;
			}
			return it->second;
		}
	};

	struct PlayAnimRequest
	{
		::Engine::ECS::Core::Entity owner;
		::Engine::FSM::Core::StateID state;
		::Game::Combat::Skill::SkillID skill;
		
		AnimID anim;


		// メタ情報
		float speed = 1.0f;
		bool loop = false;
		float start_frame = 0.0f;		// クリップの開始フレーム		(0.0fはデフォルト)
		float end_frame = -1.0f;		// クリップの再生終了フレーム	(-1.0 は未設定)

	};

	// public
	struct PlayAnimRequestBuffer
	{
		std::vector<PlayAnimRequest> reqs;
		void clear() { reqs.clear(); }
	};

	struct PlayAnimSystem
	{
		::Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(const float dt);
	};
}