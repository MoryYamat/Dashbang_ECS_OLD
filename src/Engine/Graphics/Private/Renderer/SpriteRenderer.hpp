#pragma once
#include "Engine/VFX/Public/VFXAPI.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace Engine::Graphics
{
	class OpenGLTextureManager;
	class SpriteRenderer
	{
	public:
		void Init();
		void Shutdown();

		~SpriteRenderer() { Shutdown(); }

		void Draw(
			Shader& shader,
			const glm::mat4& view,
			const glm::mat4& proj,
			const glm::mat4& model,
			GLuint glTex,
			Engine::VFX::VFXBlend blend,
			const glm::vec4& tint
		);

	private:
		GLuint vao_ = 0;
		GLuint vbo_ = 0;
		GLuint ebo_ = 0;
		GLuint program_ = 0;
	};
}