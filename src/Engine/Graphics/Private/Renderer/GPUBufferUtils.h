// GPU Buffer Utils
#pragma once

//
#include "Engine/Graphics/Private/Model/ModelData.h"

// 削除予定
namespace Engine::Graphics::Render
{
	namespace GPUBufferUtils
	{
		// 
		Engine::Graphics::Model::ModelGPU createMeshGPUBuffers(const Engine::Graphics::Model::ModelData& modeldata);
	}
}
