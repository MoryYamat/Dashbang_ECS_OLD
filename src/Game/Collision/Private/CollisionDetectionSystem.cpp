#include "Game/Collision/Public/CollisionApi.hpp"


#include "Engine/Component/Private/Logic2D/CollisionComponent.hpp"
#include "Engine/Component/Private/Tags/PlayerControllerComponent.hpp"

#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp""
#include "Engine/Component/Private/Logic2D/Transform2DComponent.hpp"

#include "Game/ECS/Public/CharacterAttribTags.h"


// Skill




// teamTag
#include "Game/ECS/Private/Component/TeamComponent.h"




//
#include "GenericShape2DConverter.hpp"
#include "GenericShape2DIntersects.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Time/Private/WorldClock.hpp"

#include "Game/Collision/Public/Types.hpp"
#include "Game/Collision/Public/CollisionMaskComponent.hpp"

#include <glm/glm.hpp>

#include <iostream>
#include <cstddef>

#include <cassert>

namespace Game::Collision
{
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::WorldSystem::Core;
	using namespace Engine::Component;

	void CollisionDetectionSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto& clock = ctx.ww.GetResource<Engine::Time::WorldClockData>();
		auto& strg = ctx.ww.GetResource<Game::Collision::CollisionResultStorage>();

		strg.Clear();


		auto ents = ViewWhere(ctx.rw, AND(All<Game::Collision::CollisionMaskComponent>{}
		, OR(Any<Logic2DTransformComponent>{}, Any<Transform2DComponent>{})));


		// もっとフィルターが必要かも
		for (std::size_t i = 0; i < ents.size(); ++i)
		{
			for (std::size_t j = i + 1; j < ents.size(); ++j)
			{
				Engine::ECS::Core::Entity eA = ents[i];
				Engine::ECS::Core::Entity eB = ents[j];

				auto& maskA = ctx.ww.Get<CollisionMaskComponent>(eA);
				auto& maskB = ctx.ww.Get<CollisionMaskComponent>(eB);

				// col mask
				if (!shouldCollideWithCat(maskA, maskB)) continue;

				// rel mask
				const auto rab = computeRelation(ctx, eA, eB);
				const auto rba = computeRelation(ctx, eB, eA);
				if (!shouldCollideWithRel(maskA, maskB, rab, rba)) continue;


				// TODO: 
				auto shapeA = MakeGenericShape2D(eA, ctx);
				auto shapeB = MakeGenericShape2D(eB, ctx);
				// 衝突判定
				if (!Intersects(shapeA, shapeB)) continue;


				auto event = CollisionContact(eA, eB, glm::vec2(0.0f), 0.0f);

				strg.Add(event);

				// 
			}
		}


	};
};


// 
//void Game::Collision::UpdateCollisionResultBuffer(Engine::ECS::EntityMgr& ecs)
//{
//	using namespace Engine::Time;
//	using namespace Game::Combat::Skill::Component;
//	using namespace Game::ECS::Component;
//	using namespace Game::Combat::HitEvent::Database;
//	using namespace Game::Combat::HitEvent::Data;
//	using namespace Game::Combat::Skill::API::External;
//
//	// assert(ecs.hasResource<Game::Collision::Data::CollisionResultBuffer>() && "CollisionResultBuffer not initialized");
//	// if(!ecs.hasResource< Game::Collision::Data::CollisionResultBuffer>()) return;
//	const auto& clock = ecs.getResource<WorldClockData>();
//	auto& hitDb = ecs.getResource<HitEventDatabase>();
//	auto& buffer = ecs.getResource<Game::Collision::CollisionResultBuffer>();
//	// auto& bus = ecs.getResource<ContactBus>();
//	// std::cout << "[CollisionDetectionSystem.cpp()]Collision Count: " << buffer.results.size() << std::endl;
//
//	// バッファクリア
//	buffer.clear();
//	// bus.clear();
//
//	//auto entities = ecs.view <
//	//	Engine::ECS::Component::Logic2D::CollisionComponent,
//	//	Game::Collision::Component::CollisionMaskComponent,
//	//	Engine::ECS::Component::Logic2D::Logic2DTransformComponent>();
//
//	// Must=>And , Any=>OR
//	// using Must = std::tuple<Game::Collision::Component::CollisionMaskComponent, Engine::ECS::Component::Logic2D::CollisionComponent>;
//	using Must = std::tuple<Game::Collision::CollisionMaskComponent>;
//
//	using Any = std::tuple<Engine::Component::Logic2DTransformComponent, Engine::Component::Transform2DComponent>;
//
//	auto entities = ecs.view(Engine::ECS::EntityMgr::FilterSpec<Must, Any>{});
//
//	// スキル発生時にTransform2DComponentを検索できていない
//	//for (auto e : entities)
//	//{
//	//	std::cout << "Entity: " << e.id << std::endl;
//	//	std::cout << "  Has Mask: " << ecs.hasComponent<Game::Collision::Component::CollisionMaskComponent>(e) << std::endl;
//	//	std::cout << "  Has Collider: " << ecs.hasComponent<Engine::ECS::Component::Logic2D::CollisionComponent>(e) << std::endl;
//	//	std::cout << "  Has Logic2DTransform: " << ecs.hasComponent<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e) << std::endl;
//	//	std::cout << "  Has Transform2D: " << ecs.hasComponent<Engine::ECS::Component::Logic2D::Transform2DComponent>(e) << std::endl;
//	//}
//
//	for (size_t i = 0; i < entities.size(); ++i)
//	{
//		for (size_t j = i + 1; j < entities.size(); ++j)
//		{
//			Engine::ECS::Entity eA = entities[i];
//			Engine::ECS::Entity eB = entities[j];
//
//
//			auto& maskA = ecs.get<Game::Collision::CollisionMaskComponent>(eA);
//			auto& maskB = ecs.get<Game::Collision::CollisionMaskComponent>(eB);
//
//
//			// std::cout << "[CollisionDetectionSystem.cpp()]: before mask judge\n";
//			// 
//
//			// カテゴリ両想い判定
//			if (!Game::Collision::shouldCollideWithCat(maskA, maskB))
//				continue;
//
//			// Relation 両想い 
//			const auto rab = Game::Collision::computeRelation(ecs, eA, eB);
//			const auto rba = Game::Collision::computeRelation(ecs, eB, eA);
//			if (!Game::Collision::shouldCollideWithRel(maskA, maskB, rab, rba)) continue;
//
//			// 衝突判定済みかどうかの記録(memo)を確認
//			const bool aIsSkill = isSkillEntity(ecs, eA);
//			const bool bIsSkill = isSkillEntity(ecs, eB);
//
//			HitboxHitMemoComponent* memoA = tryGetMemo(ecs, eA);
//			HitboxHitMemoComponent* memoB = tryGetMemo(ecs, eB);
//
//			if (aIsSkill && memoA && memoA->alreadyHit(eB)) continue;
//			if (bIsSkill && memoB && memoB->alreadyHit(eA))continue;
//
//
//			// 判定用一般化形状変換
//			auto shapeA = Game::Collision::MakeGenericShape2D(eA, ecs);
//			auto shapeB = Game::Collision::MakeGenericShape2D(eB, ecs);
//			if (!Game::Collision::Intersects(shapeA, shapeB)) continue;
//
//			//std::visit([](auto&& s) {
//			//	std::cout << "[shapeA] type: " << typeid(s).name() << std::endl;
//			//	}, shapeA);
//
//			//std::visit([](auto&& s) {
//			//	std::cout << "[shapeB] type: " << typeid(s).name() << std::endl;
//			//	}, shapeB);
//
//
//			// バッファへ追加
//				// 仮のContactInfo（後で精密な法線・深度が必要になれば拡張）
//			Game::Collision::ContactInfo info{ //.contactNormal = glm::normalize(transB.positionXZ - transA.positionXZ), 
//				.penetrationDepth = 0.0f };
//			// std::cout << "[CollisionDetectionSystem]: collider detcted" << std::endl;
//			// バッファ追加
//			buffer.add(Game::Collision::CollisionResult{ eA, eB, info });
//
//
//			// FIXME isSkillEntityの再利用
//			// =================================================================
//			// Skill & target について
//			Engine::ECS::Entity skillEnt{}, targetEnt{};
//			SkillOwnerComponent meta{};
//			if (isSkillEntity(ecs, eA) && !isSkillEntity(ecs, eB))
//			{
//				skillEnt = eA; targetEnt = eB;
//				meta = ecs.get<SkillOwnerComponent>(eA);
//				// relによって解決済みの可能性あり．よって不要かも
//				// if(meta.caster == targetEnt)
//				// => skillIDに基づいて効果を適用するかどうかをキチンと判断しなければいけない
//			}
//			else if (isSkillEntity(ecs, eB) && !isSkillEntity(ecs, eA))
//			{
//				skillEnt = eB; targetEnt = eA;
//				meta = ecs.get<SkillOwnerComponent>(eB);
//
//			}
//			else
//			{
//				continue;
//			}
//
//			// FIXME: API化
//			// 以下に HitEvent生成処理を追加
//			//HitEvent ev{};
//			//ev.skill = skillEnt;
//			//ev.target = targetEnt;
//			//ev.skillId = meta.skillId;
//			//ev.SpawnTime = clock.now;
//
//			// memo更新
//			if (aIsSkill && memoA)
//			{
//				memoA->markHit(eB);
//			}
//			if (bIsSkill && memoB)
//			{
//				memoB->markHit(eA);
//			}
//
//			Game::Combat::HitEvent::API::GenerateHitEvent(ecs, skillEnt, targetEnt, meta.skillId, clock.now, hitDb);
//
//			//hitDb.push(std::move(ev));
//		}
//	}
//}

//namespace Game::Collision::System
//{
//	using namespace Game::Combat::Skill::Component;
//
//	// TODO: 関数の場所整理
//	bool isSkillEntity(Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity entity)
//	{
//		return ecs.hasComponent<SkillOwnerComponent>(entity);
//	}
//
//}



