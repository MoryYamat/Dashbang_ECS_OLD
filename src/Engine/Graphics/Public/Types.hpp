#pragma once

#include "Engine/ECS/Public/Entity.hpp"

#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Graphics/MeshComponent.hpp"


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <filesystem>



namespace Engine::Graphics
{
	class Shader
	{
	public:
		Shader() noexcept : mProgramID(0) {}
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, const glm::vec3& vec) const;
		void setVec4(const std::string& name, const glm::vec4& vec) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;

		GLuint GetProgram() const { return mProgramID; }

		void Use() const;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		
		void reset() noexcept;

	private:
		std::string loadFile(const std::string& path);

		GLuint compileShader(GLenum type, const std::string& code);

		bool checkCompileShader(GLuint shader);

		bool checkLinkStatus(GLuint program);

		GLuint mProgramID = 0;
	};

	struct RenderContext
	{
		glm::vec3 cameraPosition = glm::vec3(1.0f);// カメラ位置
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);// カメラfrontベクトル
		glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);// カメラRightベクトル
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);// カメラUpベクトル
		glm::mat4 viewMatrix = glm::mat4(1.0f);// view行列
		glm::mat4 projectionMatrix = glm::mat4(1.0f);// 投影行列
		glm::vec4 viewport = glm::vec4(1.0f);// viewport

		void UpdateMatrices(glm::mat4& viewM, glm::mat4& projM)
		{
			viewMatrix = viewM;
			projectionMatrix = projM;
		}
	};


	struct RendererPipelineEntry
	{
		Engine::ECS::Core::Entity e;

		Engine::Component::TransformComponent* ts;
		// Engine::Component::MeshComponent* mesh;
	};
	using RendererPipeline = std::vector<RendererPipelineEntry>;



}