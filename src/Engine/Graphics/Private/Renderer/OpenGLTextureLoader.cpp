#include "Engine/Graphics/Private/Renderer/OpenGLTextureLoader.hpp"

#include <stb_image.h>

#include <filesystem>
#include <iostream>


namespace Engine::Graphics
{
	GLuint CreateTexture2D_FromFile_sRGB(const std::filesystem::path& absPath)
	{
		int w = 0, h = 0, comp = 0;

		// 必要なら flip（VFX用スプライトは上下逆に見えることがある）
		// stbi_set_flip_vertically_on_load(true);

		const std::string p = absPath.string();
		stbi_uc* rgba = stbi_load(p.c_str(), &w, &h, &comp, 4);
		if (!rgba)
		{
			std::cerr << "[TextureLoader] stb load failed: " << p
				<< " reason=" << stbi_failure_reason() << "\n";
			return 0;
		}

		GLuint tex = 0;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		GLint prevUnpack = 4;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevUnpack);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		const GLint internalFormat = GL_SRGB8_ALPHA8; // カラー用途
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, rgba);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, prevUnpack);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(rgba);
		return tex;

	}
}