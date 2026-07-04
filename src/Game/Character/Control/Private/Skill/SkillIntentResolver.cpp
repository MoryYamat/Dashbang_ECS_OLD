#include "Game/Character/Control/Public/ControlApi.hpp"
#include "Game/Character/Control/Public/IntentComponent.hpp"

#include "Game/Input/Public/InputActionComponent.hpp"
#include "Game/Combat/Skill/Public/SlotComponent.hpp"

#include "Game/Combat/Skill/Public/CoreAPI.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"

#include "Game/ECS/Public/CharacterAttribTags.h"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Character::Control
{
	using namespace Engine::WorldSystem::Query;

	using namespace Game::Input;

	using namespace Game::Combat::Skill;


	static void BuildPlayerSkillIntent(
		Engine::ECS::Core::Entity e,
		const InputActionComponent& input,
		const SkillInputBindingComponent& bindings,
		const SkillSlotComponent& slots,
		SkillIntentComponent& intent
	)
	{
		for (const auto& [action, slotIdx] : bindings.actionToSlot)
		{
			if (!input.isPressed(action))
				continue;

			auto slotIndex = static_cast<std::size_t>(slotIdx);
			if (slotIndex >= static_cast<std::size_t>(SkillSlotIndex::COUNT))
				continue;

			SkillID skill = slots.slotSkills[slotIndex];
			if (!skill.valid())
				continue;

			SkillRequest req{};
			req.skill = skill;
			req.caster = e;
			// ターゲットなどは別から
			req.targetPos = {};
			req.targetEntity = {};
			// 
			req.isQueued = false;

			// std::cout << "here" << req.skill.v << "\n";

			intent.requests.push_back(req);
		}
	}

	void UpdatePlayerSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = ViewWhere(ctx.rw, All<
			InputActionComponent,
			SkillInputBindingComponent,
			SkillSlotComponent,
			SkillIntentComponent,
			Game::ECS::Tags::PlayerCharacterTag
			>{});

		if (ents.empty())
		{
			// エラー処理
		}

		// プレイヤーキャラクター想定
		for (const auto& e : ents)
		{
			const auto& input = ctx.ww.Get<InputActionComponent>(e);
			const auto& bindings = ctx.ww.Get<SkillInputBindingComponent>(e);
			const auto& slots = ctx.ww.Get<SkillSlotComponent>(e);
			auto& intent = ctx.ww.Get<SkillIntentComponent>(e);

			intent.requests.clear();


			BuildPlayerSkillIntent(e, input, bindings, slots, intent);
		}
	}
}