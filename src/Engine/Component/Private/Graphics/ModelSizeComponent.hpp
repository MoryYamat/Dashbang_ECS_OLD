#pragma once

#include <glm/glm.hpp>

namespace Engine::Component
{
	struct ModelSizeComponent
	{
		glm::vec3 min;
		glm::vec3 max;

		// サイズの取得
		glm::vec3 GetSize() const
		{
			return max - min;
		}

		// 中心座標の取得
		glm::vec3 GetCenter() const
		{
			return (min + max) * 0.5f;
		}
	};
}