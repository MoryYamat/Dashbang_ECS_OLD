// Render Data

#pragma once

// #include "Engine/Window/Public/Window.h"

#include <GLM/glm.hpp>


namespace Engine::Graphics::Render
{
	struct RenderContext
	{
		glm::vec3 cameraPosition = glm::vec3(1.0f);// カメラ位置
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);// カメラfrontベクトル
		glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);// カメラRightベクトル
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);// カメラUpベクトル
		glm::mat4 viewMatrix = glm::mat4(1.0f);// view行列
		glm::mat4 projectionMatrix = glm::mat4(1.0f);// 投影行列
		glm::vec4 viewport = glm::vec4(1.0f);// viewport
	};

}



