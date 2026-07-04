#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

namespace Game::Character::Animation
{
	void InitAllCharacterAnimProf(Engine::WorldSystem::Core::WorldCtx& ctx);

	void UpdateAllCharacterAnimSystem(Engine::WorldSystem::Core::WorldCtx& ctx);



	// 再生したいアニメーションの候補を決める
	//struct MovementAnimationResolverSystem
	//{
	//	static void Update(Engine::ECS::EntityMgr& ecs);

	//private:
	//	// 最適化-> 参照でもよいのでは？
	//	static Game::Character::Animation::Movement::MoveAnimType
	//		calcRelativeMovementDir(const glm::vec2 facingDir, const glm::vec2 movingDir,
	//			const Game::Character::Animation::Movement::MoveAnimType prev);

	//	// TODO: 最適化
	//	// TODO: 最適化
	//	// TODO: 最適化
	//	static Game::Character::Animation::Movement::MoveAnimType
	//		quantizeTo4Dir(const Game::Character::Animation::Movement::MoveAnimType type);
	//};
}