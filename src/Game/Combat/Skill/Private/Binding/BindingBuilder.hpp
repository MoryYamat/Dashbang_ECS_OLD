#pragma once

#include "Engine/FSM/Public/FSMFwd.hpp"

#include "Game/Combat/HitBox/Public/HitboxFwd.hpp"
#include "Game/Combat/Skill/Public/CoreFwd.hpp"

#include "Game/Combat/Skill/Internal/Binding/BindingDTO.hpp"
#include "Game/Combat/Skill/Internal/Binding/BindingTypes.hpp"

#include <string>
#include <vector>
#include <optional>

namespace Game::Combat::Skill::Binding
{
	struct SkillBindingBuildErrors
	{
		std::vector<std::string> msgs;
		bool ok() const { return msgs.empty(); }
		void err(std::string m) { msgs.emplace_back(std::move(m)); }
	};

	struct SkillBindingBuildResult
	{
		SkillBindingData data;
		SkillBindingBuildErrors errs;
	};

	class SkillBindingBuilder
	{
	public:
		// catalog と SkillAxis のID を渡し作成
		SkillBindingBuilder(
			const SkillCatalog& skillCat,
			const HitBox::HitBoxCatalog& hitBoxCat,
			const Animation::AnimCatalog& animCat,
			const Engine::FSM::Core::FSMCatalog& fsmCat
		);

		void Add(const SkillHitBoxBindingDTO& dto);
		void Add_Anim(const SkillAnimationBindingDTO& dto);
		void Add_Sound(const SkillSoundBindingDTO& dto);
		void Add_vfx(const SkillVFXBindingDTO& dto);

		void AddRange(const std::vector<SkillHitBoxBindingDTO>& dtos);
		void AddRange_anim(const std::vector<SkillAnimationBindingDTO>& dtos);
		void AddRange_sound(const std::vector<SkillSoundBindingDTO>& dtos);
		void AddRange_sound(const std::vector<SkillVFXBindingDTO>& dtos);


		

		// まとめてビルド
		SkillBindingBuildResult Build() const;

	private:
		const SkillCatalog& skills_;
		const HitBox::HitBoxCatalog& hitboxes_;
		const Animation::AnimCatalog& anims_;
		const Engine::FSM::Core::FSMCatalog& fsm_;

		std::vector<SkillHitBoxBindingDTO> hbdefs_;
		std::vector<SkillAnimationBindingDTO> animdefs_;
		std::vector<SkillSoundBindingDTO> sounddefs_;
		std::vector<SkillVFXBindingDTO> vfxdefs_;

		bool buildHibBox(
			const Engine::FSM::Core::CanonicalAxis& axis,
			const SkillHitBoxBindingDTO& hbdtos,
			SkillBindingData& out,
			SkillBindingBuildErrors& errs
		) const;

		bool buildAnim
		(
			const Engine::FSM::Core::CanonicalAxis& axis,
			const SkillAnimationBindingDTO& animdtos,
			SkillBindingData& out,
			SkillBindingBuildErrors& errs
		)const;

		bool buildSound
		(
			const ::Engine::FSM::Core::CanonicalAxis& axis,
			const SkillSoundBindingDTO& sounddtos,
			SkillBindingData& out,
			SkillBindingBuildErrors& errs
		)const;

		bool buildVFX
		(
			const ::Engine::FSM::Core::CanonicalAxis& axis,
			const SkillVFXBindingDTO& vfxdtos,
			SkillBindingData& out,
			SkillBindingBuildErrors& errs
		)const;
	};
}