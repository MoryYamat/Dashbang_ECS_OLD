#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Game/00_Feature/Public/EffectFeature.hpp"

#include "Game/00_Feature/Private/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/Private/System/InputFeature.h"

#include "Game/00_Feature/Private/Collision/CollisionFeature.h"


#include "Game/00_Feature/Private/Character/State/CharacterStateFeature.hpp"



#include "Game/00_Feature/Private/System/WorldClockFeature.hpp"

#include "Game/00_Feature/Private/Combat/HitEvent/HitEventFeature.hpp"
#include "Game/00_Feature/Public/HitBoxFeature.hpp"		// ファイル名変更必要あり

// world
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/00_Feature/Private/Character/Animation/AnimationFeature.hpp"

#include <iostream>


#include "Game/FSM/Public/FSMApi.hpp"
#include "Game/Debug/Public/DebugApi.hpp"
void Game::Layer::InitializeLayerFeature::DelayedInitialization(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	// World Clock
	Game::Feature::System::WorldClockFeature::Init(ctx);

	// Input
	Game::Feature::Input::InputFeature::InitInputMapping(ctx);

	if (!Game::FSM::InitAllFSMs_Game(ctx))
	{
		std::cout << "[fsms initialization failed]\n";
	}

	// Collision
	Game::Feature::CollisionFeature::InitCollisionBuffer(ctx);

	// Anim
	Game::Feature::Character::Animation::CharacterAnimationFeature::InitProfileDatabase(ctx);

	// Audio
	Game::Feature::Audio::AudioFeature::InitGameAudioSystem(ctx);
	// VFX
	Game::Feature::VFX::VFXFeature::InitGameVFXSystem(ctx);

	// ---------- Combat -----------
	// Skill
	Game::Feature::Combat::SkillFeature::InitSkillSystem(ctx);
	// HitBox
	Game::Feature::Combat::HitBoxFeature::InitHitBoxSystem(ctx);
	// Anim
	Game::Feature::Combat::AnimFeature::InitAnimSystem(ctx);

	// 最後に呼び出し
	Game::Feature::Combat::SkillFeature::InitSkillBindingSystem(ctx);
	// -----------------------------

	// Game::FSM::Debug::SmokeTest_Movement(ctx);
}