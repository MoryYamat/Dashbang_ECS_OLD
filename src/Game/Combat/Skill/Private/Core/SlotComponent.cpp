#include "Game/Combat/Skill/Public/SlotComponent.hpp"

#include "Game/Input/Public/InputActionComponent.hpp"


#include "Game/Combat/Skill/Public/CoreApi.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreDTO.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Combat::Skill
{
	void SkillInputBindingComponent::DefaultInit()
	{
		actionToSlot.clear();
		// TODO: vvvvvvindex のズレ を修正する必要あり
		actionToSlot[Game::Input::InputAction::CastSkill1] = SkillSlotIndex::Slot0;
		actionToSlot[Game::Input::InputAction::CastSkill2] = SkillSlotIndex::Slot1;
		actionToSlot[Game::Input::InputAction::CastSkill3] = SkillSlotIndex::Slot2;
		actionToSlot[Game::Input::InputAction::CastSkill4] = SkillSlotIndex::Slot3;
	}


	static bool InitSkillSlotsFromDTO(
		const SkillSlotLoadoutDTO& dto,
		const SkillCatalog& catalog,
		SkillSlotComponent& outSlots
	)
	{

		bool ok = true;

		for (std::size_t i = 0; i < SkillSlotComponent::MaxSlots; ++i)
		{
			const std::string& name = dto.slotSkillnames[i];

			if (name.empty())
			{
				outSlots.slotSkills[i] = kInvalidSkillID;
				continue;
			}

			// catalog 
			SkillID id = catalog.FindByName(name);

			if (!id.valid())
			{
				ok = false;



				outSlots.slotSkills[i] = kInvalidSkillID;
			}
			else
			{
				outSlots.slotSkills[i] = id;
			}
		}


		return ok;
	}

	bool InitSkillSlot(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		SkillSlotLoadoutDTO& dto,
		SkillSlotComponent& outSlots
	)
	{
		const auto& catalog = ctx.ww.GetResource<SkillCatalog>();

		bool ok = true;

		ok = InitSkillSlotsFromDTO(dto, catalog, outSlots);

		if (!ok)
		{
			Engine::Log::Write(Engine::Log::Level::Error, "Skill Slot",
				"Detect an error when initializing SkillSlot (func: InitSkillSlotsFromDTO)"
			);
		}

		return ok;
	}

	// 一括初期化用 (原則使用しない) → SkillSlotを直接引数とした関数を使用すること
	bool InitSkillSlot(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& dto = ctx.ww.GetResource<SkillSlotLoadoutDTO>();
		const auto& catalog = ctx.ww.GetResource<SkillCatalog>();

		bool ok = true;

		auto ents = Engine::WorldSystem::Query::ViewWhere(ctx.rw,
			Engine::WorldSystem::Query::All<SkillSlotComponent>{});

		if (ents.empty())
		{
			ok = false;
			Engine::Log::Write(Engine::Log::Level::Error, "Skill Slot", "No Entity with SkillSlotComponent was found");
			return ok;
		}

		for (const auto& e : ents)
		{
			auto& slot = ctx.ww.Get<SkillSlotComponent>(e);

			// TODO: Slot対象EntityとloudoutDTOの対応関係の構築が必要
			// TODO: Slot対象EntityとloudoutDTOの対応関係の構築が必要

			ok &= InitSkillSlotsFromDTO(dto, catalog, slot);
		}

		if (!ok)
		{
			Engine::Log::Write(Engine::Log::Level::Error, "Skill Slot"
				, "Detect an error when initializing SkillSlot (func: InitSkillSlotsFromDTO)");
		}

		return ok;
	}

}