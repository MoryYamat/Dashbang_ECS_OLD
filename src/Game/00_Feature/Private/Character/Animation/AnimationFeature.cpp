#include "AnimationFeature.hpp"


//
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/Animation/Public/AnimationApi.hpp"
//namespace Game::Feature::Character::Animation
//{
//	namespace Anim = Game::Character::Animation;
//
//	void CharacterAnimationFeature::InitProfileDatabase(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Profile::Init::InitAnimationProfileDatabase(ecs);
//	}
//
//	void CharacterAnimationFeature::UpdateBuildAnimationQuery(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Query::BuildLocomotionAnimationQuerySystem(ecs);
//	}
//	void CharacterAnimationFeature::UpdateMovementAnimationResolver(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Resolve::Movement::MovementAnimationResolverSystem::Update(ecs);
//	}
//	void CharacterAnimationFeature::UpdateAnimationArbiterSystem(Engine::ECS::EntityMgr& ecs)
//	{
//		// Anim::AnimationArbiterSystem::Update(ecs);
//	}
//	void CharacterAnimationFeature::UpdateApplyFinalAnimationDecision(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Apply::ApplyFinalAnimationDecisionSystem::Update(ecs);
//	}
//
//}
//
//// skill
//namespace Game::Feature::Character::Animation
//{
//	namespace Anim = Game::Character::Animation;
//
//	void CharacterAnimationFeature::InitSkillProfileDatabase(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Profile::Init::InitSkillAnimProfileDatabase(ecs);
//	}
//
//	void CharacterAnimationFeature::UpdateSkillAnimationResolver(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Resolve::Skill::SkillAnimResolverSystem::Update(ecs);
//	}
//
//	void CharacterAnimationFeature::UpdateBuildSkillAnimQuerySystem(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Query::BuildSkillAnimQuerySystem(ecs);
//	}
//}
//
//// cc
//namespace Game::Feature::Character::Animation
//{
//	namespace Anim = Game::Character::Animation;
//
//
//	void CharacterAnimationFeature::InitCCProfileDatabase(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Profile::Init::InitCCAnimProfileDatabase(ecs);
//	}
//
//	void CharacterAnimationFeature::UpdateCCAnimationResolver(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Resolve::CC::CCAnimResolverSystem::Update(ecs);
//	}
//
//	void CharacterAnimationFeature::UpdateBuildCCAnimQuerySystem(Engine::ECS::EntityMgr& ecs)
//	{
//		Anim::Query::CC::BuilCCAnimationQuerySystem(ecs);
//	}
//}

// 
namespace Game::Feature::Character::Animation
{
	void CharacterAnimationFeature::InitProfileDatabase(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Character::Animation::InitAllCharacterAnimProf(ctx);
	}

	void CharacterAnimationFeature::UpdateBuildAnimationQuery(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
	}

	void CharacterAnimationFeature::UpdateLocmAnimSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Character::Animation::UpdateAllCharacterAnimSystem(ctx);
	}
}