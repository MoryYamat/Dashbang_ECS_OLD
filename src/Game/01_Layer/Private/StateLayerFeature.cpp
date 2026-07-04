#include "Game/01_Layer/Public/LayerFeature.hpp"


#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/FSM/Public/MovementAxisApi.hpp"
#include "Game/Character/FSM/Public/SkillAxisApi.hpp"


void Game::Layer::StateLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	Game::Character::FSM::Movement::UpdateMovementAll(ctx);
	Game::Character::FSM::Skill::UpdateAllSkillFSM(ctx);
}