#include "CharacterSkillFeature.hpp"

#include "Game/Character/Control/Public/ControlApi.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"



namespace Game::Feature::Character
{
	void CharacterSkillFeature::UpdateCharacterSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Character::Control::UpdatePlayerSkillIntent(ctx);
	}
}