#include "Game/00_Feature/Public/HitBoxFeature.hpp"
#include "Game/Combat/HitBox/Public/HitBoxAPI.hpp"


namespace Game::Feature::Combat
{
	void HitBoxFeature::InitHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::HitBox::InitAllHitBoxSystem(ctx);
	}

	void HitBoxFeature::UpdateHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::HitBox::UpdateAllHitBoxSystem(ctx);
	}

}