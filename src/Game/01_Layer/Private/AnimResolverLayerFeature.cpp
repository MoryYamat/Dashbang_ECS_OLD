#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Game/00_Feature/Private/Character/Animation/AnimationFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Layer
{
	void LocomotionAnimLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Feature::Character::Animation::CharacterAnimationFeature::UpdateLocmAnimSystem(ctx);
	}
}

namespace Game::Layer
{
	namespace Anim = Game::Feature::Character::Animation;

	//void LocomotionResolverLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
	//{
	//	Anim::CharacterAnimationFeature::UpdateBuildAnimationQuery(ecs);

	//	Anim::CharacterAnimationFeature::UpdateMovementAnimationResolver(ecs);

	//	Anim::CharacterAnimationFeature::UpdateAnimationArbiterSystem(ecs);

	//	Anim::CharacterAnimationFeature::UpdateApplyFinalAnimationDecision(ecs);
	//}

	//void SkillAnimationResolverLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
	//{
	//	Anim::CharacterAnimationFeature::UpdateBuildSkillAnimQuerySystem(ecs);

	//	Anim::CharacterAnimationFeature::UpdateSkillAnimationResolver(ecs);
	//}

	//void CCAnimationResolverLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
	//{
	//	Anim::CharacterAnimationFeature::UpdateBuildCCAnimQuerySystem(ecs);
	//	Anim::CharacterAnimationFeature::UpdateCCAnimationResolver(ecs);
	//}

	//void AnimResolverLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
	//{

	//}
}