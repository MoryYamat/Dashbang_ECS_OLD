

#pragma once
#include "Game/Character/Control/Public/ControlFwd.hpp"
#include "Game/Character/FSM/Public/FSMFwd.hpp"
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Game/Combat/Skill/Public/CoreFwd.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <span>

namespace Game::Combat::Skill
{
	// private
	void RegisterSkillDef(SkillCatalogBuilder& builder);


	// intent component
	struct SkillRequest
	{
		Game::Combat::Skill::SkillID skill;
		Engine::ECS::Core::Entity caster;
		glm::vec2 targetPos;
		Engine::ECS::Core::Entity targetEntity;
		bool isQueued;
	};

	// 「Charancter」の Skill 実行にかかわる「状態」を保持 (Hitbox や Anim の具体的な状態は持たない)
	// 実行状態
	struct SkillRuntimeComp
	{
		SkillID curSkill = kInvalidSkillID;
		const CanonicalSkillData* curSkillData = nullptr;
		bool isActive = false;

		Engine::FSM::Core::StateID state = Engine::FSM::Core::kInvalidState;		// casting/active/recovery etc.
		float totalElapsed = 0.f;
		float phaseElapsed = 0.f;
		float prevPhaseElapsed = 0.f;

		// 
		bool acceptedTriggerThisFrame = false;
		float curPhaseDuration = 0.f;
		bool hasPhaseDuration = false;

		// 外部入力
		bool triggerRequested = false;
		bool cancelRequested = false;

		std::vector<SkillRequest> pendingTriggers;

		void ResetInput()
		{
			triggerRequested = false;
			cancelRequested = false;
			pendingTriggers.clear();
		}

		void ResetTimeElapsed()
		{
			totalElapsed = 0.f;
			phaseElapsed = 0.f;
			prevPhaseElapsed = 0.f;
		}
	};

	


	// public
	void InitAllSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	bool InitSkillSlot(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		SkillSlotLoadoutDTO& dto,
		SkillSlotComponent& outSlots
	);

	bool InitSkillSlot(Engine::WorldSystem::Core::WorldCtx& ctx);

}