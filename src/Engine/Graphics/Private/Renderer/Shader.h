//// load shader and set datas
//#pragma once
//
//#include <GLAD/glad.h>
//
//#include <GLM/glm.hpp>
//#include <GLM/gtc/matrix_transform.hpp>
//#include <GLM/gtc/type_ptr.hpp>
//
//#include <string>
//
//
//
//namespace Engine::Graphics::Render
//{
//	class Shader
//	{
//	public:
//
//		Shader(const std::string& vertexPath, const std::string& fragmentPath);
//		~Shader();
//
//		void setBool(const std::string& name, bool value) const;
//		void setInt(const std::string& name, int value) const;
//		void setFloat(const std::string& name, float value) const;
//		void setVec3(const std::string& name, const glm::vec3& vec) const;
//		void setVec4(const std::string& name, const glm::vec4& vec) const;
//		void setMat4(const std::string& name, const glm::mat4& mat) const;
//
//		GLuint GetProgram() const { return mProgramID; }
//
//		void Use() const;
//
//	private:
//		std::string loadFile(const std::string& path);
//
//		GLuint compileShader(GLenum type, const std::string& code);
//
//		bool checkCompileShader(GLuint shader);
//
//		bool checkLinkStatus(GLuint program);
//
//		GLuint mProgramID;
//	};
//}
