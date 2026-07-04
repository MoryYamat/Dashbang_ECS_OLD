#include "GPUBufferUtils.h"

#include "Engine/Graphics/Public/GraphicsApi.hpp"
#include "Engine/Graphics/Private/Model/ModelData.h"

#include <iostream>

namespace Engine::Graphics
{
	Engine::Graphics::Model::ModelGPU createMeshGPUBuffers(const Engine::Graphics::Model::ModelData& modelData)
	{
		Engine::Graphics::Model::ModelGPU modelGPU;


		for (const auto& meshData : modelData.meshes)
		{
			Engine::Graphics::Model::MeshGPU meshGPU;

			// generate VAO, VBO, EBO
			glGenVertexArrays(1, &meshGPU.vao);
			glGenBuffers(1, &meshGPU.vbo);
			glGenBuffers(1, &meshGPU.ebo);

			glBindVertexArray(meshGPU.vao);

			// vbo
			glBindBuffer(GL_ARRAY_BUFFER, meshGPU.vbo);
			glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Engine::Graphics::Model::VertexData), 
				meshData.vertices.data(), GL_STATIC_DRAW);

			// ebo
			if (meshData.hasIndices && !meshData.indices.empty())
			{
				// インデックス数
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGPU.ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), meshData.indices.data(), GL_STATIC_DRAW);
				meshGPU.indexCount = static_cast<unsigned int>(meshData.indices.size());
			}
			else
			{
				// 頂点数
				meshGPU.indexCount = static_cast<unsigned int>(meshData.vertices.size());
			}

			using V = Engine::Graphics::Model::VertexData;
			const GLsizei stride = sizeof(V);

			// layout(location = 0) position
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, position));

			// layout(location = 1) normal
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, normal));

			// layout(location = 2) uv
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, texCoords));

			// layout(location = 3) tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, tangent));

			// layout(location = 4) bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, bitangent));

			// boneIDs/weights を使う場合は ipointer/pointer で 5/6 を設定する
			glEnableVertexAttribArray(5);
			glVertexAttribIPointer(5, 4, GL_UNSIGNED_INT, stride, (void*)offsetof(V, joints));

			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, weights));

			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, weaponMask));

			glBindVertexArray(0);
			modelGPU.meshesGPU.emplace_back(std::move(meshGPU));

		}

		std::cerr << "The modelData was successfully converted to GPUmodelData." << std::endl;

		return modelGPU;
	}
}

Engine::Graphics::Model::ModelGPU Engine::Graphics::Render::GPUBufferUtils::createMeshGPUBuffers(const Engine::Graphics::Model::ModelData& modelData)
{
	Engine::Graphics::Model::ModelGPU modelGPU;


	for (const auto& meshData : modelData.meshes)
	{
		Engine::Graphics::Model::MeshGPU meshGPU;

		// generate VAO, VBO, EBO
		glGenVertexArrays(1, &meshGPU.vao);
		glGenBuffers(1, &meshGPU.vbo);
		glGenBuffers(1, &meshGPU.ebo);

		glBindVertexArray(meshGPU.vao);

		// vbo
		glBindBuffer(GL_ARRAY_BUFFER, meshGPU.vbo);
		glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Engine::Graphics::Model::VertexData), meshData.vertices.data(), GL_STATIC_DRAW);

		// ebo
		if (meshData.hasIndices && !meshData.indices.empty())
		{
			// インデックス数
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGPU.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), meshData.indices.data(), GL_STATIC_DRAW);
			meshGPU.indexCount = static_cast<unsigned int>(meshData.indices.size());
		}
		else
		{
			// 頂点数
			meshGPU.indexCount = static_cast<unsigned int>(meshData.vertices.size());
		}

		using V = Engine::Graphics::Model::VertexData;
		const GLsizei stride = sizeof(V);

		// layout(location = 0) position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, position));

		// layout(location = 1) normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, normal));

		// layout(location = 2) uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, texCoords));

		// layout(location = 3) tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, tangent));

		// layout(location = 4) bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, bitangent));

		// boneIDs/weights を使う場合は ipointer/pointer で 5/6 を設定する
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_UNSIGNED_INT, stride,(void*)offsetof(V, joints));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride,(void*)offsetof(V, weights));

		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(V, weaponMask));


		glBindVertexArray(0);
		modelGPU.meshesGPU.emplace_back(std::move(meshGPU));

	}

	std::cerr << "The modelData was successfully converted to GPUmodelData." << std::endl;

	return modelGPU;
}