#pragma once

#include "Types.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Engine/Graphics/Public/GraphicsFwd.hpp"

#include "Engine/IO/Public/FileSystemFwd.hpp"

// need to be refactored
#include "Engine/Graphics/Private/Renderer/OpenGLTextureLoader.hpp"
#include "Engine/Graphics/Private/Renderer/OpenGLTextureManager.hpp"
#include "Engine/Graphics/Private/Renderer/SpriteRenderer.hpp"

#include <filesystem>
#include <string>
#include <span>

#include <vector>
#include <unordered_map>

namespace Engine::Graphics
{
	class ITextureProvider
	{
	public:
		virtual ~ITextureProvider() = default;

		virtual TextureHandle get_or_load_texture(
			const std::filesystem::path& absPath
		) = 0;
	};

	class OpenGLTextureManager final : public ITextureProvider
	{
	public:
		~OpenGLTextureManager() override;

		void shutdown();

		TextureHandle get_or_load_texture(const std::filesystem::path& absPath) override;

		GLuint gl_id(TextureHandle h) const;
	private:
		struct Entry
		{
			std::filesystem::path absPath;
			GLuint glTex = 0;
		};

		std::vector<Entry> entries_;		// handle.value() -> index
		std::unordered_map<std::string, TextureHandle> pathToHandle_;	// absPath.string() キー
	};
}

namespace Engine::Graphics
{


	struct RendererResources
	{
		void Init(const std::string& vert, const std::string& frag);
		void Shutdown();

		~RendererResources();

		void RenderFrame(
			::Engine::WorldSystem::Core::WorldCtx& ctx,
			const RenderContext& renderCTX,
			::Engine::IO::IPathResolver& resolver
		);

		RendererResources(const RendererResources&) = delete;
		RendererResources& operator=(const RendererResources&) = delete;
		RendererResources() = default;

	private:
		bool initialized = false;

		Engine::Graphics::OpenGLTextureManager texMgr_;
		Engine::Graphics::SpriteRenderer spriteRenderer_;
		Engine::Graphics::Shader spriteShader_;// 2D
		//Engine::VFX::VFXRuntimeCache vfxCache_;
	};

	void BuildRendererPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, RendererPipeline& out);
	void UpdateRendererAll(Engine::WorldSystem::Core::WorldCtx& ctx, 
		Engine::Graphics::Shader& shader, float aspect, RenderContext& context);

	void UpdateRendererAll(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		Engine::Graphics::Shader& shader,
		RendererResources& res,
		float aspect,
		RenderContext& context);


	Engine::Graphics::Model::ModelGPU createMeshGPUBuffers(const Engine::Graphics::Model::ModelData& modelData);


	void RenderSystem(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader& shader, std::span<RendererPipelineEntry> ents,
		float aspect, RenderContext& context);

}

namespace Engine::Graphics::Animation
{
	void AnimationSystem(Engine::WorldSystem::Core::WorldCtx& ctx);




}