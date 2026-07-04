// #include "Engine/Graphics/Private/Renderer/OpenGLTextureManager.hpp"

#include "Engine/Graphics/Public/GraphicsApi.hpp"

#include "Engine/Graphics/Public/GraphicsFwd.hpp"

#include "Engine/Graphics/Private/Renderer/OpenGLTextureLoader.hpp"

#include <string>
#include <cstdint>


namespace Engine::Graphics
{
	OpenGLTextureManager::~OpenGLTextureManager()
	{
		shutdown();
	}

	void OpenGLTextureManager::shutdown()
	{

		for (auto& e : entries_)
		{
			if (e.glTex != 0)
			{
				glDeleteTextures(1, &e.glTex);
				e.glTex = 0;
			}
		}

		entries_.clear();
		pathToHandle_.clear();
	}

	TextureHandle OpenGLTextureManager::get_or_load_texture(const std::filesystem::path& absPath)
	{
		const std::string key = absPath.string();

		if (auto it = pathToHandle_.find(key); it != pathToHandle_.end())
			return it->second;

		// GLuint
		GLuint tex = CreateTexture2D_FromFile_sRGB(absPath);
		if (tex == 0)
			return TextureHandle::Invalid();

		TextureHandle h{ static_cast<TextureHandle::rep_type>(entries_.size()) };
		entries_.push_back(Entry{ absPath, tex });
		pathToHandle_.emplace(key, h);
		return h;
	}

	GLuint OpenGLTextureManager::gl_id(TextureHandle h) const
	{
		if (!h.is_valid())
			return 0;
		const std::size_t idx = static_cast<std::size_t>(h.value());
		if (idx >= entries_.size())
			return 0;
		return entries_[idx].glTex;
	}
}