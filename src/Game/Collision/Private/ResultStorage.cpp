#include "Game/Collision/Public/Types.hpp"

#include <glm/glm.hpp>
#include <vector>

#include <cstdint>
namespace Game::Collision
{
	void CollisionResultStorage::Add(const CollisionContact& contact)
	{
		contacts_.push_back(contact);
	}

	const std::vector<CollisionContact>& CollisionResultStorage::GetAll() const
	{
		return contacts_;
	}

	// 1フレームごとに初期化
	// Initialize every frame
	void CollisionResultStorage::Clear()
	{
		contacts_.clear();
	}

	// 特定エンティティに関する衝突だけを取得
	std::vector<CollisionContact> CollisionResultStorage::GetContactsInvolving(Engine::ECS::Core::Entity entity) const
	{
		std::vector<CollisionContact> result;
		for (const auto& c : contacts_)
		{
			if (c.entityA == entity || c.entityB == entity)
			{
				result.push_back(c);
			}
		}

		return result;
	}
}