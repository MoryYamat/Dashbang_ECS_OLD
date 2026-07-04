// animation 再生

#pragma once

#include "Engine/Graphics/Private/Model/ModelData.h"

#include <string>
#include <vector>

#include <GLM/glm.hpp>

namespace Engine::Graphics::Animation
{
	class Animator
	{
	public:
		explicit Animator(const Engine::Graphics::Model::ModelData* m = nullptr) : model(m) {}
		void SetModel(const Engine::Graphics::Model::ModelData* m) { model = m; invalidate_ = true; }


		// クリップ選択
		bool SetClipByName(const std::string& name, bool loop = true);
		void SetClip(int index, bool loop = true);

		// 再生制御
		void SetSpeed(float s) { speed_ = s; }
		void ResetTime() { time_ = 0.f; }
		void Update(float dt);

		// 出力
		const std::vector<glm::mat4>& Palette() const { return palette_; }
		int ClipIndex() const { return clipIndex_; }
		float Time() const { return time_; }

	private:
		const Engine::Graphics::Model::ModelData* model = nullptr;
		int clipIndex_ = -1;
		float time_ = 0.f;
		float speed_ = 1.f;
		bool loop_ = true;

		std::vector<glm::mat4> palette_;
		bool invalidate_ = true;
	};
}