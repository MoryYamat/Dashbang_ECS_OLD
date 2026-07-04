#pragma once

#include "Game/Combat/Skill/Public/CoreFwd.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"
#include "Game/Combat/Animation/Public/AnimationFwd.hpp"
#include "Game/Audio/Generated/SoundKeys.hpp"
#include "Game/VFX/Generated/VFXKeys.hpp"

#include "Engine/FSM/Public/FSMFwd.hpp"

#include <iostream>


namespace Game::Combat::Skill::Binding
{
	struct SkillHitBoxBindingEntry
	{
		::Game::Combat::Skill::SkillID skill;
		::Engine::FSM::Core::StateID state;
		::Game::Combat::HitBox::HitBoxID hitbox;
	};

	struct SkillAnimationBindingEntry
	{
		::Game::Combat::Skill::SkillID skill;
		::Engine::FSM::Core::StateID state;
		::Game::Combat::Animation::AnimID anim;
	};

	struct SkillSoundBindingEntry
	{
		::Game::Combat::Skill::SkillID skill;
		::Engine::FSM::Core::StateID state;
		::Game::Audio::SoundKey key;
	};

	struct SkillVFXBindingEntry
	{
		::Game::Combat::Skill::SkillID skill;
		::Engine::FSM::Core::StateID state;
		::Game::VFX::VFXKey key;
		float default_scale = 1.0f;
		float ttl_override = -1.0f;			// 寿命 <0 -> VFXDef.duration
		std::uint16_t count = 1;			// 生成数
		::Engine::Math::Vec3f offset_local{ 0,0,0 };		// 相対変形(基準TRSに対して)
		bool fade = false;
	};


	// 使用するかは検討必要			
	struct ResolvedBindings
	{
		::Game::Combat::HitBox::HitBoxID hitbox;
		::Game::Combat::Animation::AnimID anim;
	};

	struct SkillBindingData
	{
		// std::vector<std::uint16_t> offs;

		std::vector<SkillHitBoxBindingEntry> hbs;		// 必要ならO(1). unordered_map or csr
		std::vector<SkillAnimationBindingEntry> abs;	
		std::vector<SkillSoundBindingEntry> sbs;
		std::vector<SkillVFXBindingEntry> vbs;
		// 将来はほかの効果も追加していく

		// 将来的にはunordered_map / CSR 
		Game::Combat::HitBox::HitBoxID resolveHitBox(
			Game::Combat::Skill::SkillID skill,
			Engine::FSM::Core::StateID state
		) const
		{
			for (const auto& e : hbs)
			{
				if (e.skill == skill && e.state == state)
				{
					return e.hitbox;
				}
			}
			return Game::Combat::HitBox::kInvalidHitBoxID;
		}


		// 将来的にはunordered_map / CSR 
		::Game::Combat::Animation::AnimID resolveAnim(
			const Game::Combat::Skill::SkillID skill,
			const Engine::FSM::Core::StateID state
		) const
		{
			// std::cout << "skill: " << skill.v << "\n";
			// std::cout << "state: " << state.v << "\n";

			for (const auto& e : abs)
			{
				if (e.skill == skill && e.state == state)
				{
					return e.anim;
				}
			}
			return Game::Combat::Animation::kInvalidAnimID;
		}

		::Game::Audio::SoundKey resolveSound(
			const Game::Combat::Skill::SkillID skill,
			const Engine::FSM::Core::StateID state
		)const
		{
			for (const auto& e : sbs)
			{
				if (e.skill == skill && e.state == state)
				{
					return e.key;
				}
			}
			return	Game::Audio::SoundKey::COUNT;
		}

		const SkillVFXBindingEntry* try_resolveVFX
		(
			const Game::Combat::Skill::SkillID skill,
			const Engine::FSM::Core::StateID state
		)const
		{
			for (const auto& e : vbs)
			{
				if (e.skill == skill && e.state == state)
				{
					return &e;
				}
			}
			return nullptr;
		}
	};


}