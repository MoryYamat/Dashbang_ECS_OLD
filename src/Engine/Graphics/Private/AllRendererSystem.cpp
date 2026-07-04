#include "Engine/Graphics/Public/GraphicsApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/IO/Public/FileSystemAPI.hpp"

#include "Engine/Graphics/Public/Types.hpp"

#include "Engine/VFX/Public/VFXAPI.hpp"

#include <iostream>

namespace Engine::Graphics
{
	void UpdateRendererAll(Engine::WorldSystem::Core::WorldCtx& ctx,
		Engine::Graphics::Shader& shader, float aspect, RenderContext& context)
	{
		RendererPipeline pipeline;
		BuildRendererPipeline(ctx, pipeline);
		
		// animation 
		Engine::Graphics::Animation::AnimationSystem(ctx);

		// render
		RenderSystem(ctx, shader, pipeline, aspect, context);


	}

	void UpdateRendererAll(
		Engine::WorldSystem::Core::WorldCtx& ctx, 
		Engine::Graphics::Shader& shader, 
		RendererResources& res,
		float aspect, 
		RenderContext& context)
	{
		RendererPipeline pipeline;
		BuildRendererPipeline(ctx, pipeline);

		auto& table = ctx.ww.GetResource<::Engine::IO::MountTable>();
		::Engine::IO::VfsResolver resolver(table);

		// animation 
		Engine::Graphics::Animation::AnimationSystem(ctx);

		// render
		RenderSystem(ctx, shader, pipeline, aspect, context);

		res.RenderFrame(ctx, context, resolver);
	}
}