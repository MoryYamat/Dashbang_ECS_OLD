// ModelData (Universal Data Types)
#pragma once

#include <cfloat>
#include <vector>
#include <string>

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define MAX_BONE_INFLUENCE 4

namespace Engine::Graphics::Model
{
	struct TextureData
	{
		GLuint id = 0;
		std::string type;// "diffuse", "specular", "normal", etc.
		std::string path;


		TextureData() = default;
		~TextureData()
		{
			if (id != 0)
			{
				glDeleteTextures(1, &id);
			}
		}

		TextureData(const TextureData&) = delete;
		TextureData& operator= (const TextureData&) = delete;

		TextureData(TextureData&& o) noexcept
			: id(std::exchange(o.id, 0)), type(std::move(o.type)), path(std::move(o.path)) { }


		TextureData& operator=(TextureData&& o) noexcept {
			if (this != &o) {
				if (id) glDeleteTextures(1, &id);
				id = std::exchange(o.id, 0);
				type = std::move(o.type);
				path = std::move(o.path);
			}
			return *this;
		}
	};

	struct MaterialData
	{
		std::vector<TextureData> textures;
		glm::vec3 baseColor = glm::vec3(1.0f);

		MaterialData() = default;
		~MaterialData() = default;

		MaterialData(const MaterialData&) = delete;
		MaterialData& operator=(const MaterialData&) = delete;

		MaterialData(MaterialData&&) noexcept = default;
		MaterialData& operator=(MaterialData&&) noexcept = default;
	};

	// CPU-side data
	struct VertexData
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::uvec4 joints = { 0,0,0,0 };
		glm::vec4 weights = { 1,0,0,0 };

		//int boneIDs[MAX_BONE_INFLUENCE] = { 0 };
		//float weights[MAX_BONE_INFLUENCE] = { 0 };

		float weaponMask = 0.0f;
	};

	// ================= skelton / animation ================= 
	struct Bone
	{
		int parent = -1;
		int nodeIndex = -1;
		glm::mat4 invBind = glm::mat4(1.0f);// 逆行列

		glm::vec3 defT{ 0,0,0 };
		glm::quat defR{ 1,0,0,0 };
		glm::vec3 defS{ 1,1,1 };
		
		std::string name;
	};

	struct Skeleton
	{
		std::vector<Bone> bones;

		// glTF ノード index -> ボーンindex (逆引き)
		std::unordered_map<int, int> nodeToBone;

		std::unordered_map<std::string, int> nameToBone;
	};

	enum class ChannelType { T, R, S };

	struct Channel
	{
		int bone = -1;
		ChannelType type;
		std::vector<float> times;	// 秒
		std::vector<glm::vec3> v3;	// T/S用
		std::vector<glm::quat> vq;	// R用
	};

	struct AnimationClip
	{
		std::string name;
		float duration = 0.0f;
		std::vector<Channel> channels;
	};

	struct MeshData
	{
		std::vector<VertexData> vertices;
		std::vector<unsigned int> indices;
		bool hasIndices = false;

		MaterialData materialData;

		MeshData() = default;
		~MeshData() = default;

		MeshData(const MeshData&) = delete;
		MeshData& operator=(const MeshData&) = delete;

		MeshData(MeshData&&) noexcept = default;
		MeshData& operator=(MeshData&&) noexcept = default;

		std::string source_name;		// glTF mesh.name
		int source_mesh_index = -1;		// mi
		int source_prim_index = -1;		// pi

	};

	// 
	struct ModelData
	{
		std::vector<MeshData> meshes;

		// model's size datas
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(-FLT_MAX);
		
		// 追加
		glm::mat4 meshBindGlobal = glm::mat4(1.0f);
		glm::mat4 skeletonRootBindGlobal = glm::mat4(1.0f);

		Skeleton skeleton;
		std::vector<AnimationClip> clips;

		

		ModelData() = default;
		~ModelData() = default;

		ModelData(const ModelData&) = delete;
		ModelData& operator=(const ModelData&) = delete;

		ModelData(ModelData&&) noexcept = default;
		ModelData& operator=(ModelData&&) noexcept = default;

		glm::vec3 GetSize() const { return max - min; }
		glm::vec3 GetCenter() const { return (min + max) * 0.5f; }

	};

	// GPU-side data
	struct MeshGPU
	{
		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ebo = 0;
		unsigned int indexCount = 0;

		MeshGPU() = default;
		~MeshGPU() {
			if (ebo) glDeleteBuffers(1, &ebo);
			if (vbo) glDeleteBuffers(1, &vbo);
			if (vao) glDeleteVertexArrays(1, &vao);
		}

		MeshGPU(const MeshGPU&) = delete;
		MeshGPU& operator=(const MeshGPU&) = delete;

		MeshGPU(MeshGPU&& o) noexcept {
			*this = std::move(o);
		}
		MeshGPU& operator=(MeshGPU&& o) noexcept {
			if (this != &o) {
				// 先に自分の分を片付け
				if (ebo) glDeleteBuffers(1, &ebo);
				if (vbo) glDeleteBuffers(1, &vbo);
				if (vao) glDeleteVertexArrays(1, &vao);

				vao = std::exchange(o.vao, 0);
				vbo = std::exchange(o.vbo, 0);
				ebo = std::exchange(o.ebo, 0);
				indexCount = o.indexCount;
			}
			return *this;
		}
	};

	struct ModelGPU
	{
		std::vector<MeshGPU> meshesGPU;
	};
}

