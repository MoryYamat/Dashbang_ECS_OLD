#include "BindingBuilder.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/AxisComponent.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"

#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"

#include "Game/Combat/Skill/Internal/Binding/BindingTypes.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"
#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"

#include "Game/Combat/Animation/Internal/AnimTypes.hpp"
#include "Game/Combat/Animation/Public/AnimationFwd.hpp"

#include "Game/Audio/Generated/SoundKeys.hpp"
#include "Game/VFX/Generated/VFXKeys.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <string_view>
#include <cstdint>
#include <optional>

namespace
{

}

namespace Game::Combat::Skill::Binding
{
	using namespace Engine::FSM::Core;

	SkillBindingBuilder::SkillBindingBuilder(
		const SkillCatalog& skillCat,
		const HitBox::HitBoxCatalog& hitBoxCat,
		const Animation::AnimCatalog& animCat,
		const Engine::FSM::Core::FSMCatalog& fsmCat
	) : skills_(skillCat)
		, hitboxes_(hitBoxCat)
		, anims_(animCat)
		, fsm_(fsmCat)
	{

	}

	void SkillBindingBuilder::Add(const SkillHitBoxBindingDTO& hitboxdto)
	{
		hbdefs_.push_back(hitboxdto);
	}

	void SkillBindingBuilder::AddRange(const std::vector<SkillHitBoxBindingDTO>& hbdtos)
	{
		hbdefs_.insert(hbdefs_.end(), hbdtos.begin(), hbdtos.end());
	}

	void SkillBindingBuilder::Add_Anim(const SkillAnimationBindingDTO& animdto)
	{
		animdefs_.push_back(animdto);
	}

	
	void SkillBindingBuilder::AddRange_anim(const std::vector<SkillAnimationBindingDTO>& animdtos)
	{
		animdefs_.insert(animdefs_.end(), animdtos.begin(), animdtos.end());
	}

	void SkillBindingBuilder::Add_Sound(const SkillSoundBindingDTO& dto)
	{
		sounddefs_.push_back(dto);
	}

	void SkillBindingBuilder::Add_vfx(const SkillVFXBindingDTO& dto)
	{
		vfxdefs_.push_back(dto);
	}

	void SkillBindingBuilder::AddRange_sound(const std::vector<SkillSoundBindingDTO>& dtos)
	{
		sounddefs_.insert(sounddefs_.end(), dtos.begin(), dtos.end());
	}

	SkillBindingBuildResult SkillBindingBuilder::Build() const
	{
		SkillBindingBuildResult result;

		// Skill FSM Canonical Data の 取得
		std::string_view axisName = AxisTraits<Game::Character::FSM::Skill::SkillTag>::AxisName();
		AxisID skillAxisId = Engine::FSM::Core::AxisLookup::FindAxisIdxByName(fsm_, axisName);
		{
			if (skillAxisId.v >= fsm_.axes.size())
			{
				Engine::Log::Write(Engine::Log::Level::Error,
					"SkillBindingBuilder",
					"Skill axis FSM index out of range.");
				result.errs.err("[SkillBinding] Skill axis FSM index out of range");
				return result;
			}
		}
		const auto& axis = fsm_.axes[skillAxisId.v];

		for (const auto& dto : hbdefs_)
		{
			buildHibBox(axis, dto, result.data, result.errs);
		}

		for (const auto& dto : animdefs_)
		{
			buildAnim(axis, dto, result.data, result.errs);
		}

		for (const auto& dto : sounddefs_)
		{
			buildSound(axis, dto, result.data, result.errs);
		}

		for (const auto& dto : vfxdefs_)
		{
			buildVFX(axis, dto, result.data, result.errs);
		}

		return result;
	}

	bool SkillBindingBuilder::buildHibBox(
		const Engine::FSM::Core::CanonicalAxis& axis,
		const SkillHitBoxBindingDTO& hbdtos,
		SkillBindingData& out,
		SkillBindingBuildErrors& errs
	) const
	{
		
		// Skill名 → SkillID
		SkillID skillId = skills_.FindByName(hbdtos.key.skill);
		{
			if (!skillId.valid())
			{
				errs.err("[SkillBinding] Unknown skill name: '" + hbdtos.key.skill +
					"' (state='" + hbdtos.key.state +
					"' hitbox='" + hbdtos.hitbox + "')");

				return false;
			}
		}

		// State名 → StateID (Skill軸のFSMから)
		std::uint32_t id = Engine::FSM::Core::AxisLookup::FindStateIdx(axis, hbdtos.key.state);
		StateID stateId(id);
		{
			if (!stateId.valid())
			{
				errs.err("[SkillBinding] Unknown state name: '" + hbdtos.key.state +
					"' (skill='" + hbdtos.key.skill +
					"' hitbox='" + hbdtos.hitbox + "')");
				return false;
			}
		}
		
		// HitBox名 → HitBoxID
		HitBox::HitBoxID hitBoxId = hitboxes_.FindByName(hbdtos.hitbox);
		{
			if (!hitBoxId.valid())
			{
				errs.err("[SkillBinding] Unknown HitBox name: '" + hbdtos.key.state+
					"' (state='" + hbdtos.key.skill +
					"' hitbox='" + hbdtos.hitbox + "')");
				return false;
			}
		}


		// TODO: キー拡張によるエラー検出処理変更 (現行仕様ではエラー)
		// ★ (skill, state) の重複チェック
		for (const auto& e : out.hbs)
		{
			if (e.skill == skillId && e.state == stateId)
			{
				errs.err(
					"[SkillBinding] Duplicate binding for "
					"(skill='" + hbdtos.key.skill +
					"', state='" + hbdtos.key.state +
					"') : existingHitBoxId=" + std::to_string(e.hitbox.v) +
					", newHitBox='" + hbdtos.hitbox + "'"
				);
				return false; // 2つ目以降は無視
			}
		}


		// 正常に解決できたらエントリ追加
		SkillHitBoxBindingEntry hbentry;
		hbentry.skill = skillId;
		hbentry.state = stateId;
		hbentry.hitbox = hitBoxId;

		// 追加
		out.hbs.push_back(hbentry);

		return true;
	}

	bool SkillBindingBuilder::buildAnim
	(
		const Engine::FSM::Core::CanonicalAxis& axis,
		const SkillAnimationBindingDTO& animdtos,
		SkillBindingData& out,
		SkillBindingBuildErrors& errs
	)const
	{
		// Skill名 → SkillID
		SkillID skillId = skills_.FindByName(animdtos.key.skill);
		{
			if (!skillId.valid())
			{
				errs.err("[SkillBinding] Unknown skill name: '" + animdtos.key.skill +
					"' (state='" + animdtos.key.state +
					"' hitbox='" + animdtos.anim + "')");

				return false;
			}
		}

		// State名 → StateID (Skill軸のFSMから)
		std::uint32_t id = Engine::FSM::Core::AxisLookup::FindStateIdx(axis, animdtos.key.state);
		StateID stateId(id);
		{
			if (!stateId.valid())
			{
				errs.err("[SkillBinding] Unknown state name: '" + animdtos.key.state +
					"' (skill='" + animdtos.key.skill +
					"' hitbox='" + animdtos.anim + "')");
				return false;
			}
		}

		// Animation名 → AnimID
		Animation::AnimID animID = anims_.FindByName(animdtos.anim);
		{
			if (!animID.valid())
			{
				errs.err("[SkillBinding] Unknown HitBox name: '" + animdtos.key.state +
					"' (state='" + animdtos.key.skill +
					"' hitbox='" + animdtos.anim + "')");
				return false;
			}
		}


		// TODO: キー拡張によるエラー検出処理変更 (現行仕様ではエラー)
		// ★ (skill, state) の重複チェック
		for (const auto& e : out.abs)
		{
			if (e.skill == skillId && e.state == stateId)
			{
				errs.err(
					"[SkillBinding] Duplicate binding for "
					"(skill='" + animdtos.key.skill +
					"', state='" + animdtos.key.state +
					"') : existingHitBoxId=" + std::to_string(e.anim.v) +
					", newHitBox='" + animdtos.anim + "'"
				);
				return false; // 2つ目以降は無視
			}
		}


		// 正常に解決できたらエントリ追加
		SkillAnimationBindingEntry animEntry;
		animEntry.skill = skillId;
		animEntry.state = stateId;
		animEntry.anim = animID;
		
		std::cout << "(skill:'" << skillId.v << "' state:'"<< stateId.v << "')-> anim:'" << animID.v << "\n";

		// 追加
		out.abs.push_back(animEntry);


		return true;
	}

	bool SkillBindingBuilder::buildSound
	(
		const ::Engine::FSM::Core::CanonicalAxis& axis,
		const SkillSoundBindingDTO& sounddtos,
		SkillBindingData& out,
		SkillBindingBuildErrors& errs
	)const
	{
		// Skill名 → SkillID
		SkillID skillId = skills_.FindByName(sounddtos.key.skill);
		{
			if (!skillId.valid())
			{
				errs.err("[SkillBinding] Unknown skill name: '" + sounddtos.key.skill +
					"' (state='" + sounddtos.key.state +
					"' sound ='" + sounddtos.debug_name + "')");

				return false;
			}
		}

		// State名 → StateID (Skill軸のFSMから)
		std::uint32_t id = Engine::FSM::Core::AxisLookup::FindStateIdx(axis, sounddtos.key.state);
		StateID stateId(id);
		{
			if (!stateId.valid())
			{
				errs.err("[SkillBinding] Unknown state name: '" + sounddtos.key.state +
					"' (skill='" + sounddtos.key.skill +
					"' sound='" + sounddtos.debug_name+ "')");
				return false;
			}
		}

		// Animation名 → AnimID
		::Game::Audio::SoundKey soundKey = sounddtos.sound;
		{
			if (soundKey == ::Game::Audio::SoundKey::COUNT)
			{
				errs.err("[SkillBinding] Unknown HitBox name: '" + sounddtos.key.state +
					"' (state='" + sounddtos.key.skill +
					"' sound='" + sounddtos.debug_name+ "')");
				return false;
			}
		}


		// TODO: キー拡張によるエラー検出処理変更 (現行仕様ではエラー)
		// ★ (skill, state) の重複チェック
		for (const auto& e : out.sbs)
		{
			if (e.skill == skillId && e.state == stateId)
			{
				errs.err(
					"[SkillBinding] Duplicate binding for "
					"(skill='" + sounddtos.key.skill +
					"', state='" + sounddtos.key.state +
					"') : existingSoundKey=" + sounddtos.debug_name
				);
				return false; // 2つ目以降は無視
			}
		}


		// 正常に解決できたらエントリ追加
		SkillSoundBindingEntry soundEntry;
		soundEntry.skill = skillId;
		soundEntry.state = stateId;
		soundEntry.key = soundKey;

		std::cout << "(skill: '" << skillId.v << "' state:'" << stateId.v << "')-> sound:'" << sounddtos.debug_name << "'\n";
		

		// 追加
		out.sbs.push_back(soundEntry);

		return true;
	}

	bool SkillBindingBuilder::buildVFX
	(
		const ::Engine::FSM::Core::CanonicalAxis& axis,
		const SkillVFXBindingDTO& vfxdtos,
		SkillBindingData& out,
		SkillBindingBuildErrors& errs
	)const
	{
		// Skill名 → SkillID
		SkillID skillId = skills_.FindByName(vfxdtos.key.skill);
		{
			if (!skillId.valid())
			{
				errs.err("[SkillBinding] Unknown skill name: '" + vfxdtos.key.skill +
					"' (state='" + vfxdtos.key.state +
					"' sound ='" + vfxdtos.debug_name + "')");

				return false;
			}
		}

		// State名 → StateID (Skill軸のFSMから)
		std::uint32_t id = Engine::FSM::Core::AxisLookup::FindStateIdx(axis, vfxdtos.key.state);
		StateID stateId(id);
		{
			if (!stateId.valid())
			{
				errs.err("[SkillBinding] Unknown state name: '" + vfxdtos.key.state +
					"' (skill='" + vfxdtos.key.skill +
					"' sound='" + vfxdtos.debug_name + "')");
				return false;
			}
		}

		// Animation名 → AnimID
		::Game::VFX::VFXKey vfxKey= vfxdtos.vfx;
		{
			if (vfxKey == ::Game::VFX::VFXKey::Count)
			{
				errs.err("[SkillBinding] Unknown HitBox name: '" + vfxdtos.key.state +
					"' (state='" + vfxdtos.key.skill +
					"' sound='" + vfxdtos.debug_name + "')");
				return false;
			}
		}


		// TODO: キー拡張によるエラー検出処理変更 (現行仕様ではエラー)
		// ★ (skill, state) の重複チェック
		for (const auto& e : out.vbs)
		{
			if (e.skill == skillId && e.state == stateId)
			{
				errs.err(
					"[SkillBinding] Duplicate binding for "
					"(skill='" + vfxdtos.key.skill +
					"', state='" + vfxdtos.key.state +
					"') : existingVFXKey=" + vfxdtos.debug_name
				);
				return false; // 2つ目以降は無視
			}
		}


		// 正常に解決できたらエントリ追加
		SkillVFXBindingEntry vfxEntry;
		vfxEntry.skill = skillId;
		vfxEntry.state = stateId;
		vfxEntry.key = vfxKey;
		vfxEntry.default_scale = vfxdtos.default_scale;
		vfxEntry.count = vfxdtos.count;
		vfxEntry.offset_local = vfxdtos.offset_local;
		vfxEntry.ttl_override = vfxdtos.ttl_override;
		vfxEntry.fade = vfxdtos.fade;

		std::cout << "(skill: '" << skillId.v << "' state:'" << stateId.v << "')-> vfx:'" << vfxdtos.debug_name << "'\n";

		// 追加
		out.vbs.push_back(vfxEntry);

		return true;
	}
}