//
#pragma once

#include "Engine/Component/Private/Logic2D/CollisionComponent.hpp"
#include "Engine/Component/Private/Logic2D/Transform2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/WorldSystem/Private/Core/WorldAccess.hpp"
#include "Engine/WorldSystem/Private/Core/WorldCtx.hpp"

#include "Game/Collision/Private/GenericShape2D.hpp"


#include <glm/glm.hpp>

namespace Game::Collision
{
	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算
	Game::Collision::GenericShape2D BuildGenericShape2D(const Engine::Component::Collider& collider
		, const Engine::Component::Logic2DTransformComponent& transform);

	// Skill 仕様確定後作成
	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 
	//Game::Collision::GenericShape2D BuildGenericShape2D(const Game::Combat::Skill::Component::Attack2DShape& attack2DArea
	//	, const Engine::Component::Transform2DComponent& worldTransform);

	// (GenericShape2Dを作成)
	template<typename ShapeT, typename TransformT>
	Game::Collision::GenericShape2D MakeGenericShape2DFromTransform(
		const ShapeT& shape,
		const TransformT& transform
	)
	{
		return BuildGenericShape2D(shape, transform);
	}

	// (Transformの型が未知の場合)GenericShape2Dを作成するインターフェース
	template<typename ShapeT>
	Game::Collision::GenericShape2D MakeGenericShape2DAuto(
		const ShapeT& shape,
		Engine::ECS::Core::Entity e,
		Engine::WorldSystem::Core::WorldCtx& ctx
	)
	{
		if (ctx.rw.Has<Engine::Component::Logic2DTransformComponent>(e)) {
			return MakeGenericShape2DFromTransform(shape, ctx.ww.Get<Engine::Component::Logic2DTransformComponent>(e));
		}
		else if (ctx.rw.Has<Engine::Component::Transform2DComponent>(e)) {
			return MakeGenericShape2DFromTransform(shape, ctx.ww.Get<Engine::Component::Transform2DComponent>(e));
		}
		else {
			assert(false && "No valid transform component");
			return Game::Collision::GenericShape2D{};
		}
	}

	// GenericShape2Dを取得するインターフェース
	Game::Collision::GenericShape2D MakeGenericShape2D(
		Engine::ECS::Core::Entity e,
		Engine::WorldSystem::Core::WorldCtx& ctx);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::Component::Logic2DTransformComponent& transform);

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::Component::Transform2DComponent& transform);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localDir, const Engine::Component::Logic2DTransformComponent& transform);

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localDir, const Engine::Component::Transform2DComponent& transform);
}