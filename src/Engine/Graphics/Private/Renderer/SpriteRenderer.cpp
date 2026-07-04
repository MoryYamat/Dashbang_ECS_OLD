#include "Engine/Graphics/Private/Renderer/SpriteRenderer.hpp"

#include "Engine/Graphics/Public/Types.hpp"

namespace Engine::Graphics
{
	void SpriteRenderer::Init()
	{
		const float vertices[] =
		{
			// pos (x,y,z)
			-0.5f, 0.0f, -0.5f,		0.f, 0.f,
			0.5f, 0.0f, -0.5f,		1.f, 0.f,
			0.5f, 0.0f,  0.5f,		1.f, 1.f,
			-0.5f, 0.0f, 0.5f,		0.f, 1.f
		};
		const unsigned int indices[] = { 0,2,1,  0,3,2 };

		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);
		glGenBuffers(1, &ebo_);

		glBindVertexArray(vao_);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindVertexArray(0);
	}

	void SpriteRenderer::Shutdown()
	{
		if (vao_) glDeleteVertexArrays(1, &vao_);
		if (vbo_) glDeleteBuffers(1, &vbo_);
		if (ebo_) glDeleteBuffers(1, &ebo_);
		vao_ = vbo_ = ebo_ = 0;
	}

	void SpriteRenderer::Draw(
		Shader& shader,
		const glm::mat4& view,
		const glm::mat4& proj,
		const glm::mat4& model,
		GLuint glTex,
		Engine::VFX::VFXBlend blend,
		const glm::vec4& tint
	)
	{
		// blend
		glEnable(GL_BLEND);
		if (blend == Engine::VFX::VFXBlend::Add)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		else
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 深度テストは「地面に貼る」ならONでOK（ただし半透明は順序問題がある）
		// glEnable(GL_DEPTH_TEST);

		shader.Use();
		shader.setMat4("uModel", model);
		shader.setMat4("uView", view);
		shader.setMat4("uProjection", proj);
		shader.setVec4("uTint", tint);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glTex);
		shader.setInt("uTex", 0);

		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);

	}
}