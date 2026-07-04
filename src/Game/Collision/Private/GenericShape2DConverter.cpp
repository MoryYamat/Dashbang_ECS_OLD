#include "GenericShape2DConverter.hpp"

#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/CollisionComponent.hpp"

#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <variant>

namespace Game::Collision
{
	using namespace Engine::Component;
	using namespace Engine::Math::Logic2D;

	// GenericShape2Dを取得するインターフェース
	Game::Collision::GenericShape2D MakeGenericShape2D(
		Engine::ECS::Core::Entity e,
		Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		if (ctx.rw.Has<CollisionComponent>(e))
		{
			const auto& coll = ctx.ww.Get<CollisionComponent>(e);
			if (ctx.rw.Has<Logic2DTransformComponent>(e))
			{
				return MakeGenericShape2DFromTransform(coll.collider, ctx.ww.Get<Logic2DTransformComponent>(e));
			}
		}


		return GenericShape2D{};
	}

	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算
	Game::Collision::GenericShape2D BuildGenericShape2D(const Engine::Component::Collider& collider
		, const Engine::Component::Logic2DTransformComponent& transform)
	{
		return std::visit([&](auto&& shape) -> GenericShape2D
			{
				using T = std::decay_t<decltype(shape)>;

				if constexpr (std::is_same_v<T, Circle2D>)
				{
					glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
					return FromCircle(center, shape.radius);
				}
				else if constexpr (std::is_same_v<T, Obb2D>)
				{
					glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
					glm::vec2 axisX = glm::normalize(shape.axisX);
					glm::vec2 axisZ = glm::normalize(shape.axisZ);
					glm::vec2 halfExtents = shape.halfExtents * transform.scale;
					return FromObb(center, halfExtents, axisX, axisZ);
				}
				else if constexpr (std::is_same_v<T, Box2D>)
				{
					glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
					glm::vec2 halfExtents = shape.halfExtents * transform.scale;
					return FromObb(center, halfExtents, transform.right, transform.front);
				}
				else
				{
					return GenericShape2D{};
				}
			}, collider.shape);
	}

	// Skill 仕様確定後作成
	// ローカルCollisionとワールド変換からワールド上の抽象判定形状を計算 
	//Game::Collision::GenericShape2D BuildGenericShape2D(const Game::Combat::Skill::Component::Attack2DShape& attack2DArea
	//	, const Engine::Component::Transform2DComponent& worldTransform)
	//{
	//	return GenericShape2D{};
	//}

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::Component::Logic2DTransformComponent& transform)
	{
		return Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.rotation, transform.scale);
	}

	// ローカル形状のオフセットを適用して中心座標のワールド変換を取得する
	glm::vec2 applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::Component::Transform2DComponent& transform)
	{
		return Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.rotationY, glm::vec2(transform.scale, transform.scale));
	}

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localDir, const Engine::Component::Logic2DTransformComponent& transform)
	{
		return Transform::TransformDirection(localDir, transform.rotation);
	}

	// ローカル形状の方向ベクトルをとワールド変換情報から方向ベクトルのワールド変換
	glm::vec2 transformLocalPointToWorld(const glm::vec2& localDir, const Engine::Component::Transform2DComponent& transform)
	{
		return Transform::TransformDirection(localDir, transform.rotationY);
	}
}