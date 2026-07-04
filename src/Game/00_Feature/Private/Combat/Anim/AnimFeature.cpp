#include "Game/00_Feature/Public/HitboxFeature.hpp"

#include "Game/Combat/Animation/Public/AnimationAPI.hpp"



namespace Game::Feature::Combat
{

	void AnimFeature::InitAnimSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Animation::InitAllAnimSystem(ctx);
	}

	void AnimFeature::UpdateAnimSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Animation::UpdateAllAnimSystem(ctx);

	}
}