#include "Engine/Graphics/Public/GraphicsApi.hpp"

#include "Engine/Graphics/Public/Types.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Common/TransformComponent.hpp"

namespace Engine::Graphics
{
	void BuildRendererPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, RendererPipeline& out)
	{
		out.clear();
		ctx.ww.ForEachAlive([&](Engine::ECS::Core::Entity e)
			{
				auto* ts = ctx.ww.TryGet<Engine::Component::TransformComponent>(e);
				if (!ts) return;


				out.push_back(RendererPipelineEntry{
					e,
					ts
					});
			});
	}
}