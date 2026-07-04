#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Game/00_Feature/Private/Character/Movement/MovementFeature.h"
#include "Game/00_Feature/Private/Character/Control/Skill/CharacterSkillFeature.hpp"

#include "Game/00_Feature/Private/Combat/Skill/SkillFeature.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <iostream>


void Game::Layer::IntentLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	Game::Feature::Character::MovementFeature::UpdateMovementIntent(ctx);
	Game::Feature::Character::MovementFeature::UpdateLogicFacing(ctx);

	Game::Feature::Character::CharacterSkillFeature::UpdateCharacterSkillIntent(ctx);
}