#pragma once

#include "Game/Audio/Generated/SoundKeys.hpp"
#include "Game/VFX/Generated/VFXKeys.hpp"
#include "Engine/Math/Public/MathAPI.hpp"
#include <string>
#include <cstdint>

namespace Game::Combat::Skill::Binding
{
	// TODO: 効果チャネル化/効果配列化 etc.
	// キー
	struct SkillStateKeyDTO
	{
		std::string skill;		// SkillDefDTO.name
		std::string state;		// FSM の state名 ("Casting","Active",...)
	};

	// HitBox 用 DTO
	struct SkillHitBoxBindingDTO
	{
		SkillStateKeyDTO key;
		std::string hitbox;		// HitBoxDTO.name
	};

	// Animation 再生区間
	struct PlaySection
	{
		// -1 は 未設定
		float start = -1.0f;
		float end = -1.0f;
	};

	// Animation 用 DTO
	struct SkillAnimationBindingDTO
	{
		SkillStateKeyDTO key;
		std::string anim;		// AnimDTO.name
		// std::string layer;
		PlaySection section;
		bool loop = false;
		float speed = 1.0f;
		float startTime = 0.0f;
		float endTime = 0.0f;
		bool loopWithinRange = false;
		// loop, speed, etc,...
	};

	// sound 用 DTO
	struct SkillSoundBindingDTO
	{
		SkillStateKeyDTO key;
		Game::Audio::SoundKey sound;		// SoundKey
		std::string debug_name;				// デバッグ用
		float volume_scale = 1.0f;
		
		
		// volume, 3D/2D, etc...
	};



	struct SkillVFXBindingDTO
	{
		SkillStateKeyDTO key;
		Game::VFX::VFXKey vfx;				// vfxKey
		std::string debug_name;				// debug
		float default_scale = 1.0f;
		float ttl_override = -1.0f;			// 寿命 <0 -> VFXDef.duration
		std::uint16_t count = 1;			// 生成数
		::Engine::Math::Vec3f offset_local{ 0,0,0 };		// 相対変形(基準TRSに対して)
		bool fade = false;
	};
}