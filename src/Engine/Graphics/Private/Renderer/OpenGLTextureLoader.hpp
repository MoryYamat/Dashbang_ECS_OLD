#pragma once

#include <filesystem>
#include <glad/glad.h>

namespace Engine::Graphics
{
	GLuint CreateTexture2D_FromFile_sRGB(const std::filesystem::path& absPath);
}