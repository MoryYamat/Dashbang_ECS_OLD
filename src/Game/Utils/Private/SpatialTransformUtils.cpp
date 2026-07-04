#include "Game/Utils/Public/GameUtilsApi.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Graphics/Private/Renderer/RenderContext.h"



#include "Game/Utils/Public/GameUtilsApi.hpp"
#include "Engine/Graphics/Public/Types.hpp"


namespace Game::Utils
{
	glm::vec2 ProjectScreenToLogicXZPlane(const glm::vec2& screenMousePos,
		const Engine::Graphics::RenderContext& renderContext)
	{
		// Screen coordinates → NDC → world ray (near/far)
// スクリーン座標 → NDC → ワールドレイ（near/far） (マウスポインタのRAY)
		glm::vec3 nearPoint = glm::unProject(glm::vec3(screenMousePos.x, renderContext.viewport.w - screenMousePos.y, 0.0f),
			renderContext.viewMatrix, renderContext.projectionMatrix, renderContext.viewport);

		glm::vec3 farPoint = glm::unProject(glm::vec3(screenMousePos.x, renderContext.viewport.w - screenMousePos.y, 1.0f),
			renderContext.viewMatrix, renderContext.projectionMatrix, renderContext.viewport);

		// 正規化の安全性を高める処理を実装する必要がある
		// nan(not a Number)が入る可能性がある.
		// nan(not a Number)が入る可能性がある.
		// nan(not a Number)が入る可能性がある.
		// マウスレイ （のベクトル)
		glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);
		// 0除算防止
		if (rayDir.y == 0.0f) return glm::vec2(0.0f);// ここでリターンしている理由→ rayDir.y == 0.0f;
		// nearPoint.y + t * rayDir.y = (y=0)
		float t = -nearPoint.y / rayDir.y;
		// 地面(Y=0)との交差点 
		glm::vec3 intersection = nearPoint + t * rayDir;

		glm::vec2 mouseWorldXZ(intersection.x, intersection.z);

		// Engine::Debug::Logging::LogVector_string("SpatialTransformUtils.cpp(convert)", mouseWorldXZ);

		return mouseWorldXZ;
	}
}
