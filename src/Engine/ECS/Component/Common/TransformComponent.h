// Position, rotation and scale information in world space
// ワールド空間での、位置・回転・スケールの情報
#pragma once

#include "Engine/Config/CanonicalDefaults.h"

#include "Engine/ECS/Component/Graphics/ModelSizeComponent.h"

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::ECS::Component::Common
{
	struct TransformComponent
	{
		glm::vec3 position = glm::vec3(0.0f);
		// need to convert to a quaternion
		// degreesで保持し，内部の行列生成はglm::radians()で変換する
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);


		glm::mat4 toMatrix() const
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

			// 現在，論理は+Z基準
			// モデル補正値：(論理は-Z基準)モデルは+Z正面なので180°追加(将来はより最適化する必要あり)
			// モデル補正値：(論理は-Z基準)モデルは+Z正面なので180°追加(将来はより最適化する必要あり)
			// モデル補正値：(論理は-Z基準)モデルは+Z正面なので180°追加(将来はより最適化する必要あり)
			// モデル補正値：(論理は-Z基準)モデルは+Z正面なので180°追加(将来はより最適化する必要あり)
			// モデル補正値：(論理は-Z基準)モデルは+Z正面なので180°追加(将来はより最適化する必要あり)
			// constexpr float modelFacingOffsetDeg = 180.0f;
			// 現在論理は+Z基準

			// 補正値
			constexpr float modelFacingOffsetDeg = 0.0f;

			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
			model = glm::rotate(model, glm::radians(rotation.y + modelFacingOffsetDeg), glm::vec3(0, 1, 0));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
			model = glm::scale(model, scale);
			return model;
		}

		// 実際に表示されている大きさの取得
		// Get the actual size of the image
		glm::vec3 GetWorldSize(const Engine::ECS::Component::Graphics::ModelSizeComponent& modelSize) const
		{
			return modelSize.GetSize() * scale;
		}
	};

}


