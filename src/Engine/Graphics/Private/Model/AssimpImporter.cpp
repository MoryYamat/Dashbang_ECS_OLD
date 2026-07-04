// #include "AssimpImporter.h"

// #include <iostream>


// Engine::Graphics::Model::ModelData Engine::Graphics::Model::AssimpImporter::Import(const std::string& path)
// {
// 	ModelData data;

// 	Assimp::Importer importer;
// 	const aiScene* scene = importer.ReadFile(path,
// 		aiProcess_Triangulate |
// 		aiProcess_GenSmoothNormals |
// 		aiProcess_FlipUVs |
// 		aiProcess_CalcTangentSpace);
// 	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
// 	{
// 		std::cerr << "[AssimpImporter.cpp(Import)]: Error Assimp: " << importer.GetErrorString() << std::endl;
// 		return data;
// 	}
// 	else
// 	{
// 		std::cerr << "[AssimpImporter.cpp(Import)]: MODEL LOAD STARTING... " << std::endl;
// 	}

// 	processNode(scene->mRootNode, scene, data);

// 	std::cerr << "[AssimpImporter.cpp(Import)]: Load Completed: [[\"" << path << "\"]]" << std::endl;


// 	std::cerr << "[AssimpImporter.cpp(Import)]: Model center: " << data.GetCenter().x << ", " << data.GetCenter().y << ", " << data.GetCenter().z << std::endl;
// 	std::cerr << "[AssimpImporter.cpp(Import)]: Model size: " << data.GetSize().x << ", " << data.GetSize().y << ", " << data.GetSize().z << std::endl;



// 	return data;
// }


// void Engine::Graphics::Model::AssimpImporter::processNode(aiNode* node, const aiScene* scene, ModelData& modelData)
// {
// 	for (unsigned int i = 0; i < node->mNumMeshes; i++)
// 	{
// 		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

// 		modelData.meshes.push_back(processMesh(mesh, scene, modelData));
// 	}

// 	for (unsigned int i = 0; i < node->mNumChildren; i++)
// 	{
// 		processNode(node->mChildren[i], scene, modelData);
// 	}
// }

// Engine::Graphics::Model::MeshData Engine::Graphics::Model::AssimpImporter::processMesh(aiMesh* mesh, const aiScene* scene, Engine::Graphics::Model::ModelData& modelData)
// {
// 	//static int x = 1;
// 	//std::cout << "[assimpimporter.cpp]: mesh num:" << x << std::endl;
// 	//x++;


// 	MeshData meshData;
// 	VertexData vertexData;

// 	glm::vec3 vector;

// 	//　頂点属性データ
// 	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
// 	{
// 		vector.x = mesh->mVertices[i].x;
// 		vector.y = mesh->mVertices[i].y;
// 		vector.z = mesh->mVertices[i].z;
// 		vertexData.position = vector;

// 		// update bounding box' data
// 		modelData.min = glm::min(modelData.min, vector);
// 		modelData.max = glm::max(modelData.max, vector);

// 		if (mesh->HasNormals())
// 		{

// 		}

// 		if (mesh->mTextureCoords[0])
// 		{

// 		}
// 		else
// 		{

// 		}
// 		meshData.vertices.push_back(vertexData);
// 	}

// 	// インデックスデータ
// 	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
// 	{
// 		aiFace face = mesh->mFaces[i];

// 		for (unsigned int j = 0; j < face.mNumIndices; j++)
// 		{
// 			meshData.indices.push_back(face.mIndices[j]);
// 			//std::cout << "inputed indices: " << face.mIndices[j] << std::endl;
// 		}
// 	}

// 	// indicesがあればTrue, なければFalse;
// 	meshData.hasIndices = !meshData.indices.empty();


// 	//std::cout << "[assimpimporter.cpp(processMesh)]: Meshes max vertex is:" << modelData.max.x << ", " << modelData.max.y << ", " << modelData.max.z << std::endl;
// 	//std::cout << "[assimpimporter.cpp(processMesh)]: Meshes min vertex is:" << modelData.min.x << ", " << modelData.min.y << ", " << modelData.min.z << std::endl;

// 	return meshData;
// }