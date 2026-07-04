#include "Engine/Sync/Public/SyncApi.hpp"

#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Engine::Sync
{
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::Component;

	void Apply2DTransform(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = ViewWhere(ctx.rw, All<Logic2DTransformComponent, TransformComponent>{});

		for (const auto& e : ents)
		{
			const auto& logic = ctx.ww.Get<Logic2DTransformComponent>(e);
			auto& transform = ctx.ww.Get<TransformComponent>(e);

			transform. position.x = logic.positionXZ.x;
			transform. position.y = 0.0f;// 
			transform. position.z = logic.positionXZ.y;
			transform.rotation.y = glm::degrees(logic.GetRotationYFromFrontVector());
		}

		
	}
}