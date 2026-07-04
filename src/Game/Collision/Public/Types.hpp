#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include <glm/glm.hpp>

namespace Game::Collision
{
	// あたり判定には過剰な情報
	enum class ContactChannel : uint8_t
	{
		Solid,
		Hitbox
	};// 物理 or ヒット

	enum class ContactPhase : uint8_t
	{
		Begin,
		Stay,
		End
	};//

	struct ContactPayload
	{
		glm::vec2 normal{ 0,0 };
		float depth = 0.0f;

		int skillId = -1;
		int hitboxId = -1;
		glm::vec2 impactDir{ 0,0 };
		float impactMag = 0.0f;
	};

	struct Contact
	{
		Engine::ECS::Core::Entity a;
		Engine::ECS::Core::Entity b;
		ContactChannel channel;
		ContactPhase phase;
		ContactPayload payload;
		std::uint64_t key; // (a,b,channel) ハッシュ
	};
	// あたり判定には過剰な情報

	struct ContactBus
	{
		std::vector<Contact> begin, stay, end;
		void clear() { begin.clear(); stay.clear(); end.clear(); }
		void push(const Contact& c)
		{
			if (c.phase == ContactPhase::Begin)
			{
				begin.push_back(c);
			}
			else if (c.phase == ContactPhase::Stay)
			{
				stay.push_back(c);
			}
			else
			{
				end.push_back(c);
			}
		}
	};

	// CollisionResultStorageへ統合後 廃止予定
	//// 衝突の詳細情報(法線方向など)
	struct ContactInfo
	{
		glm::vec2 contactNormal;// 衝突面法線方向
		float penetrationDepth;// めり込み量
	};

	// 衝突結果
	struct CollisionResult
	{
		Engine::ECS::Core::Entity entityA;// 衝突体A
		Engine::ECS::Core::Entity entityB;// 衝突体B
		ContactInfo contact;// 衝突の詳細情報(法線方向など)
	};



	// Bufferデータ(毎フレーム更新)
	struct CollisionResultBuffer
	{
		std::vector<CollisionResult> results;
		void clear() { results.clear(); }
		void add(const CollisionResult& result) { results.push_back(result); }
	};
	// CollisionResultStorageへ統合後 廃止予定

	struct CollisionContact
	{
		Engine::ECS::Core::Entity entityA;// 衝突体A
		Engine::ECS::Core::Entity entityB;// 衝突体B
		glm::vec2 contactNormal;// 衝突面法線方向
		float penetrationDepth;// めり込み量

		CollisionContact(Engine::ECS::Core::Entity a, Engine::ECS::Core::Entity b, const glm::vec2& normal = glm::vec2(0.0f), float penetration = 0.0f)
			: entityA(a), entityB(b), contactNormal(normal), penetrationDepth(penetration) {}
	};

	// 1フレームのコリジョン情報バッファ
	class CollisionResultStorage
	{
	public:

		// 
		void Add(const CollisionContact& contact);

		// 
		const std::vector<CollisionContact>& GetAll() const;

		// 1フレームごとに初期化
		// Initialize every frame
		void Clear();

		// 特定エンティティに関する衝突だけを取得
		std::vector<CollisionContact> GetContactsInvolving(Engine::ECS::Core::Entity entity) const;

	private:
		std::vector<CollisionContact> contacts_;


		// unordered_multimap<Entity, CollisionContact> を併用して検索最適化

		// FrameScopedStorage<CollisionResultStorage> などでライフサイクル管理の明示化
	};
}