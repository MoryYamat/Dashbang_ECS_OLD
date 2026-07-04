// クリップ評価 & パレット生成
#pragma once

// #include "Engine/Graphics/Private/Model/ModelData.h"

#include "Engine/Graphics/Public/GraphicsFwd.hpp"

#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>


namespace Engine::Graphics::Animation
{
	struct TRS
	{
		glm::vec3 t{ 0,0,0 };
		glm::quat r{ 1, 0,0,0 };
		glm::vec3 s{ 1,1,1 };
	};

	void SampleClip
	(
		const Engine::Graphics::Model::ModelData& model,
		const Engine::Graphics::Model::AnimationClip& clip,
		float time,
		std::vector<TRS>& outLocalTRS,
		bool wrap
	);

	void BuildPalette
	(
		const Engine::Graphics::Model::ModelData& model,
		const std::vector<TRS>& local,
		std::vector<glm::mat4>& outPalette
	);
}