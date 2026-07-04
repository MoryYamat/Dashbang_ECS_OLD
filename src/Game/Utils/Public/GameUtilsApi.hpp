#pragma once

#include <glm/glm.hpp>
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

namespace Game::Utils
{
	glm::vec2 ProjectScreenToLogicXZPlane(const glm::vec2& screenMousePos, 
		const Engine::Graphics::RenderContext& renderContext);
}