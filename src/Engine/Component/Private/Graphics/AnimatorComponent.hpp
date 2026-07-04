#pragma once

#include "Engine/Graphics/Private/Animation/AnimationEval.hpp"
#include "Engine/Graphics/Private/Model/ModelData.h"

#include <cstddef>
#include <vector>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>

namespace Engine::Component
{
	struct AnimatorComponent
	{
		int clipIndex = -1;
		float time = 0.f;
		float speed = 1.f;
		bool loop = true;

		std::vector<glm::mat4> palette;

		std::vector<Engine::Graphics::Animation::TRS> localTRS;

		// サブ区間
		float startTime = 0.f;
		float endTime = -1.f;		// seconds <0 means "use clip end"
		bool loopWithinRange = false;		// true なら [start, end)でループ
	};

	inline bool SetClipByName
	(
		const Engine::Graphics::Model::ModelData& model,
		AnimatorComponent& a,
		std::string_view name,
		bool loop = true
	)
	{
		for (int i = 0; i < (int)model.clips.size(); ++i)
		{
			if (model.clips[i].name == name)
			{
				a.clipIndex = i;
				a.time = 0.f;
				a.loop = loop;
				return true;
			}
		}


		return false;
	}

	inline bool SetClipByNameRange(
		const ::Engine::Graphics::Model::ModelData& model,
		AnimatorComponent& a,
		std::string_view name,
		bool loop,
		float startTimeSec,
		float endTimeSec,
		bool loopWithinRange
	)
	{
		for (int i = 0; i < (int)model.clips.size(); ++i)
		{
			if (model.clips[i].name == name)
			{
				a.clipIndex = i;
				a.loop = loop;

				a.startTime = std::max(0.0f, startTimeSec);
				a.endTime = endTimeSec;
				a.loopWithinRange = loopWithinRange;

				// 再生開始をstartへ
				a.time = a.startTime;
				return true;
			}
		}

		return false;
	}
}