#pragma once

#include "Engine/Graphics/Private/Model/ModelData.h"

#include "cgltf.h"

#include "stb_image.h"

// #include <glad/glad.h>

#include <string>

namespace Engine::Graphics::Model
{
	class CgltfImporter
	{
	public:
		static Engine::Graphics::Model::ModelData Import(const std::string& path);


		static void ImportAnimationsInto(const std::string& animGlbPath, ModelData& base);


	private:
		static bool GetImageBytes_FromBufferView(
			const cgltf_image* img,
			const cgltf_data* g,
			const unsigned char*& bytes,
			size_t& size
		);

		static GLuint CreateGLTexture2D_FromMemory_sRGB(
			const unsigned char* bytes, size_t size
		);

		static glm::mat4 LocalOf(const cgltf_node* n);

		static void BuildNodeGlobals(
			const cgltf_data* g,
			std::vector<int>& parent,
			std::vector<glm::mat4>& globals
		);


	};
}