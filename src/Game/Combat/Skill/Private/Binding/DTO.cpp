#include "Game/Combat/Skill/Internal/Binding/BindingDTO.hpp"

#include "Game/Combat/Skill/Private/Binding/BindingBuilder.hpp"

#include "Game/Combat/Skill/Internal/Binding/BindingTypes.hpp"

#include "Game/Audio/Generated/SoundKeys.hpp"
#include "Game/VFX/Generated/VFXKeys.hpp"
namespace
{
	using namespace Game::Combat::Skill::Binding;
	void make_BindingDTO(SkillBindingBuilder& builder)
	{
		SkillHitBoxBindingDTO test_skill_hb;
		test_skill_hb.key.skill = "test_skill";
		test_skill_hb.key.state = "Active";
		test_skill_hb.hitbox = "default_obb2D";
		SkillAnimationBindingDTO test_skill_anim;
		test_skill_anim.key.skill = "test_skill";
		test_skill_anim.key.state = "Active";
		test_skill_anim.anim = "slash_outward_default";
		test_skill_anim.loop = false;
		test_skill_anim.speed = 1.0f;
		test_skill_anim.startTime = 0.0f;
		test_skill_anim.endTime = -1.0f;
		test_skill_anim.loopWithinRange = false;
		SkillSoundBindingDTO test_skill_sound;
		test_skill_sound.key.skill = "test_skill";
		test_skill_sound.key.state = "Active";
		test_skill_sound.sound = Game::Audio::SoundKey::sfx_test;
		test_skill_sound.volume_scale = 1.0f;
		test_skill_sound.debug_name = "sfx_test";
		SkillVFXBindingDTO test_skill_vfx;
		test_skill_vfx.key.skill = "test_skill";
		test_skill_vfx.key.state = "Active";
		test_skill_vfx.vfx = Game::VFX::VFXKey::sprite_test;
		test_skill_vfx.debug_name = "sprite_test";
		test_skill_vfx.offset_local = { 0.0f, 1.0f, 1.0f};
		test_skill_vfx.ttl_override = 5.0f;


		builder.Add(test_skill_hb);
		builder.Add_Anim(test_skill_anim);
		builder.Add_Sound(test_skill_sound);
		builder.Add_vfx(test_skill_vfx);
	}
}

namespace Game::Combat::Skill::Binding
{
	void RegisterSkillBindingDTO(Game::Combat::Skill::Binding::SkillBindingBuilder& builder)
	{
		make_BindingDTO(builder);
	}
}