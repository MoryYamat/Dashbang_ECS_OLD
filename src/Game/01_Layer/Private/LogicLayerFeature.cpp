#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Game/00_Feature/Private/Character/Movement/MovementFeature.h"

#include "Game/00_Feature/Private/Collision/CollisionFeature.h"

#include "Game/00_Feature/Private/Combat/HitEvent/HitEventFeature.hpp"

#include "Game/00_Feature/Private/Combat/Skill/SkillFeature.h"

#include "Game/00_Feature/Private/Character/State/CharacterStateFeature.hpp"

#include "Game/00_Feature/Private/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Game/00_Feature/Private/ECS/ECSGeneral.hpp"

#include "Game/00_Feature/Private/System/WorldClockFeature.hpp"

#include "Game/00_Feature/Public/HitBoxFeature.hpp"

// world
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Sync/Public/SyncApi.hpp"


void Game::Layer::LogicLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	// lifetime
	Game::Feature::ECS::GameGeneralSytem::UpdateLifetimeSystem(ctx);

	// position
	Game::Feature::Character::MovementFeature::UpdateLogicPosition(ctx);
	Engine::Sync::Apply2DTransform(ctx);

	// collision
	Game::Feature::CollisionFeature::UpdateCollisionSystem(ctx);

	// skill
	Game::Feature::Combat::SkillFeature::UpdateSkillRuntimeSystem(ctx);

	// hitbox
	Game::Feature::Combat::HitBoxFeature::UpdateHitBoxSystem(ctx);
	// animation
	::Game::Feature::Combat::AnimFeature::UpdateAnimSystem(ctx);
}