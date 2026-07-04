#include "Game/Combat/Skill/Internal/Core/CoreDTO.hpp"

#include "Game/Combat/Skill/Private/Core/SkillCatalogBuilder.hpp"

namespace Game::Combat::Skill
{
	static void make_skillDTO(SkillCatalogBuilder& builder)
	{
		SkillDefDTO test_skill;

		test_skill.fsm = "Basic";
		test_skill.name = "test_skill";
		test_skill.profile = "Default";
		test_skill.phases = {
			SkillPhaseDTO{.name = "Casting", .duration = 0.2f}, 
			SkillPhaseDTO{.name = "Active", .duration = 0.4f}, 
			SkillPhaseDTO{.name = "Recovery", .duration = 0.2f} };
		test_skill.effects = {
			SkillEffectDefDTO{.phase = "Active", .timeOffset = 0.1f, .lifetime = 0.3f, .kind = "SpawnHitbox"},
			SkillEffectDefDTO{.phase = "Active", .timeOffset = 0.0f, .lifetime = 0.3f, .kind = "ModifyMoveSpeed", .value = 0.0},
			SkillEffectDefDTO{.phase = "Active", .timeOffset = 0.0f, .kind = "PlayAnim"},
			SkillEffectDefDTO{.phase = "Active", .timeOffset = 0.0f, .kind = "PlaySFX"},
			SkillEffectDefDTO{.phase = "Active", .timeOffset = 0.2f, .kind = "PlayVFX"},// TODO: VFXのlifetime定義もここで行うようにする
		};

		builder.add(std::move(test_skill));
	}

	void RegisterSkillDef(SkillCatalogBuilder& builder)
	{
		make_skillDTO(builder);
	}
}

namespace Game::Combat::Skill
{
	static void make_Slot2SkillDTO(SkillSlotLoadoutDTO& out)
	{
		out.slotSkillnames = { "test_skill" };
	}

}