// A component that stores data on the minimum and maximum coordinates of the model space.
// モデル空間の最小座標、最大座標のデータを格納するコンポーネント

#include <GLM/glm.hpp>
namespace Engine::ECS::Component::Graphics
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

