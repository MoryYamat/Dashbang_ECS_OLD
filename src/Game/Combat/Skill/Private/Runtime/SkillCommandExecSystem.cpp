// AnimatorComponent へ Request を発行
// SpawnHitbox/ へ CMD を発行
// Engine/Audio/ へ CMD を 発行

#include "Game/Combat/Skill/Public/RuntimeAPI.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"

// hitbox
#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"

// anim
#include "Game/Combat/Animation/Internal/AnimTypes.hpp"
#include "Game/Combat/Animation/Public/AnimationFwd.hpp"

// sound
#include "Game/Audio/Generated/SoundKeys.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"
#include "Game/Audio/Public/GameAudioAPI.hpp"

// vfx
#include "Game/VFX/Generated/VFXKeys.hpp"
#include "Engine/VFX/Public/VFXAPI.hpp"
#include "Game/VFX/Public/GameVFXAPI.hpp"

#include "Game/Combat/Skill/Internal/Binding/BindingTypes.hpp"

#include "Game/Combat/Skill/Internal/Runtime/RuntimeTypes.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Log/Public/LogApi.hpp"

#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"

// math
#include "Engine/Math/Public/MathAPI.hpp"
#include "Engine/Math/Private/GlmBridge.hpp"// need to be refactored

#include "Game/Combat/Skill/Internal/SkillLog.hpp"

#include <iostream>

namespace Game::Combat::Skill::Runtime
{
	void SkillCommandExecSystem::Update(const float dt)
	{
		auto& buf = ctx.ww.GetResource<SkillLogicCommandBuffer>();

		// if (buf.cmds.empty())
		// return;

		for (const auto& cmd : buf.cmds)
		{
			switch (cmd.kind)
			{
			case LogicCommandKind::SpawnHitBox:
				HandleSpawnHitBox(cmd);
				break;
			case LogicCommandKind::ModifyMoveSpeed:
				HandleModifyMoveSpeed(cmd);
				break;
			case LogicCommandKind::PlayAnim:
				HandlePlayerAnim(cmd);// 未作成
				break;
			case LogicCommandKind::PlaySFX:
			{
				HandleSkillSFX(cmd);
				break;
			}
			case LogicCommandKind::PlayVFX:
			{
				HandleSkillVFX(cmd);
				break;
			}
			}
		}
	}

	using namespace Game::Combat::HitBox;

	// サブシステム用リクエストに変換
	void SkillCommandExecSystem::HandleSpawnHitBox(const SkillLogicCommand& cmd)
	{
		auto& buffer = ctx.ww.GetResource<HitBoxSpawnRequestBuffer>();
		auto& binding = ctx.ww.GetResource<Binding::SkillBindingData>();

		HitBoxID hb = binding.resolveHitBox(cmd.skill, cmd.state);
		if (!hb.valid())
		{
			Engine::Log::Write(Engine::Log::Level::Error, "SkillCommandExecSystem", 
				"HitboxMap failed to resolve HitBoxID");
			return;
		}


		HitBoxSpawnRequest r{};
		r.hitbox = hb;
		r.owner = cmd.owner;
		r.skill = cmd.skill;
		r.state = cmd.state;
		r.triggerTime = cmd.effectTime;
		r.lifetime = cmd.lifetime;
		// 将来：キャラごとの設定やスキル種別で HitboxID を解決して入れる

		// std::cout << "here1\n";

		buffer.reqs.push_back(r);
	}

	void SkillCommandExecSystem::HandleModifyMoveSpeed(const SkillLogicCommand& cmd)
	{
		using namespace Game::Character::FSM::Movement;

		auto* mods = ctx.ww.TryGet<MovementModifierComponent>(cmd.owner);
		if (!mods)
		{
			return;
		}

		MovementModifierEntry e{};
		e.mul = cmd.value;
		e.add = 0.f;
		e.remaining = cmd.lifetime;

		mods->entries.push_back(e);
	}

	// サブシステム用リクエストに変換
	void SkillCommandExecSystem::HandlePlayerAnim(const SkillLogicCommand& cmd)
	{
		using namespace ::Game::Combat::Animation;

		auto& buf = ctx.ww.GetResource<PlayAnimRequestBuffer>();
		auto& binding = ctx.ww.GetResource<Binding::SkillBindingData>();


		AnimID id = binding.resolveAnim(cmd.skill, cmd.state);
		if (!id.valid())
		{
			//TODO: エラー種別を判定する方法を考えるべき → skill / state どちらに 不一致があるのか
			
			Engine::Log::Write(Engine::Log::Level::Error, "SkillCommandExecSystem",
				"AnimMap failed to resolve AnimID");
			return;
		}
		// 
		// AnimPlayRequest r{};
		// r.owner = cmd.owner;
		// r.skill = cmd.skill;
		// r.state = cmd.state;
		// // 将来：キャラの種類やスキルから AnimClip を引く
		// 
		// animReqBuf.reqs.push_back(r);

		PlayAnimRequest r{};
		r.anim = id;
		r.owner = cmd.owner;
		r.skill = cmd.skill;
		r.state = cmd.state;

		buf.reqs.push_back(std::move(r));

		// std::cout << "here2222\n";
	}

	void SkillCommandExecSystem::HandleSkillSFX(const SkillLogicCommand& cmd)
	{
		using namespace ::Engine::Audio;

		auto& buf = ctx.ww.GetResource<AudioCmdBufferResource>().cmd;
		const auto& ids = ctx.rw.GetResource<::Game::Audio::AudioIds>();
		auto& binding = ctx.ww.GetResource<Binding::SkillBindingData>();

		::Game::Audio::SoundKey key = binding.resolveSound(cmd.skill, cmd.state);
		if (key == ::Game::Audio::SoundKey::COUNT)
		{
			//TODO: エラー種別を判定する方法を考えるべき → skill / state どちらに 不一致があるのか
			Engine::Log::Write(Engine::Log::Level::Error, "SkillCommandExecSystem",
				"SoundMap failed to resolve SoundKey");
			return;
		}

		::Engine::Audio::SoundID id = ids.get(key);
		if (!id.is_valid())
		{
			Engine::Log::Write(Engine::Log::Level::Error, "SkillCommandExecSystem",
				"Invalid SoundID.");
			return;
		}

		// std::cerr << "SFX requested:'" <<  "'" << "\n";		// for debug

		buf.play_one_shot(id);

	}

	void SkillCommandExecSystem::HandleSkillVFX(const SkillLogicCommand& cmd)
	{
		using namespace ::Engine::VFX;
		using namespace ::Engine::Math;

		auto& buf = ctx.ww.GetResource<VFXCmdBufferResource>();
		const auto& ids = ctx.ww.GetResource<::Game::VFX::VFXIds>();
		auto& binding = ctx.ww.GetResource<Binding::SkillBindingData>();

		const auto* e = binding.try_resolveVFX(cmd.skill, cmd.state);
		if (!e)
		{
			return;
		}


		const auto  key = e->key;
		if (key == Game::VFX::VFXKey::Count)
		{
			Log::warn(Log::kRuntimeSystem, "VFXMap failed to resolve VFXKey");
			return;
		}

		
		const auto id = ids.get(key);
		if (!id.is_valid())
		{
			Log::warn(Log::kRuntimeSystem, "Invalid VFXID");
			return;
		}

		// 基準TRS: まずはownerからとる(あとでhitbox基準に変える)
		//const Engine::Math::TRS basis = 
		
		// TODO: 座標系の変換問題
		// Sprite系の描画はいったん見送り
		// need to be refactored
		// auto pos = detail::glm_bridge::FromGlm(cmd.logic2d->positionXZ);
		// auto right = detail::glm_bridge::FromGlm(cmd.logic2d->right);
		// auto front = detail::glm_bridge::FromGlm(cmd.logic2d->front);
		// auto yawRad = cmd.logic2d->rotation;
		// 
		// Vec2f worldoffset = { right * e->offset_local.x + front * e->offset_local.z };
		// 
		// ::Engine::VFX::VFXCmd vfxReq;
		// vfxReq.id = id;
		// vfxReq.trs.translation = { pos.x + worldoffset.x , e->offset_local.y, pos.y + worldoffset.y};
		// vfxReq.trs.rotation = Engine::Math::FromYaw(yawRad);
		// vfxReq.ttlOverride = e->ttl_override;

		// buf.cmd.add(vfxReq);
	}


}