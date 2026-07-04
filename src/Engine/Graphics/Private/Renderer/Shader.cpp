#include "Engine/Graphics/Public/Types.hpp"

#include <GLAD/glad.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

namespace Engine::Graphics
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
		: mProgramID(0)
	{
		std::string vertexCode = loadFile(vertexPath);
		std::string fragmentCode = loadFile(fragmentPath);

		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

		mProgramID = glCreateProgram();
		glAttachShader(mProgramID, vertexShader);
		glAttachShader(mProgramID, fragmentShader);
		glLinkProgram(mProgramID);

		if (!checkLinkStatus(mProgramID))
		{
			std::cerr << "[Shader.cpp(shader)]: Failed to link program" << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//std::cerr << "[Shader.cpp(shader)]: Shader initialization completed successfully." << std::endl;
	}

	void Shader::reset() noexcept
	{
		if (mProgramID != 0)
		{
			glDeleteProgram(mProgramID);
			mProgramID = 0;
		}
	}

	Shader::~Shader()
	{
		//if (mProgramID != 0) glDeleteProgram(mProgramID);
		reset();
	}

	Shader::Shader(Shader&& other) noexcept
		: mProgramID(other.mProgramID)
	{
		other.mProgramID = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this != &other)
		{
			if (mProgramID != 0) glDeleteProgram(mProgramID);
			mProgramID = other.mProgramID;
			other.mProgramID = 0;
		}
		return *this;
	}



	void Shader::Use() const
	{
		glUseProgram(mProgramID);
	}

	// location setter

	void Shader::setBool(const std::string& name, bool value) const
	{
		// The value -1 will be returned if if name does not correspond to an
		// active uniform variable name in program, or if name is associated with a named
		// uniform block. (from ref.)
		GLint loc = glGetUniformLocation(mProgramID, name.c_str());
		if (loc == -1) return;// 
		glUniform1i(loc, (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const
	{
		GLint loc = glGetUniformLocation(mProgramID, name.c_str());
		if (loc == -1) return;

		glUniform1i(loc, value);
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		GLint loc = glGetUniformLocation(mProgramID, name.c_str());
		if (loc == -1)
		{
			std::cerr << "[Shader] setFloat missing uniform: " << name
				<< " program=" << mProgramID << "\n";
			return;
		}
		glUniform1f(loc, value);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
	{
		GLint loc = glGetUniformLocation(mProgramID, name.c_str());
		if (loc == -1) return;
		glUniform3fv(loc, 1, &vec[0]);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4& vec) const
	{
		GLint loc = glGetUniformLocation(mProgramID, name.c_str());
		if (loc == -1) return;
		glUniform4fv(loc, 1, &vec[0]);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		GLint loc = glGetUniformLocation(mProgramID, name.c_str());
		if (loc == -1) return;
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
	}



	std::string Shader::loadFile(const std::string& path)
	{
		std::ifstream file(path);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	GLuint Shader::compileShader(GLenum type, const std::string& code)
	{
		GLuint shader = glCreateShader(type);
		const char* source = code.c_str();
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		if (!checkCompileShader(shader))
		{
			std::cerr << "[Shader.cpp]: Compilation Failed." << std::endl;
		}

		return shader;
	}

	bool Shader::checkCompileShader(GLuint shader)
	{
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "[Shader.cpp]: Compile Error: " << infoLog << std::endl;
			return false;
		}

		return true;
	}

	bool Shader::checkLinkStatus(GLuint program)
	{
		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cerr << "[Shader.cpp]: Link error: " << infoLog << std::endl;
			return false;
		}

		return true;
	}
}

