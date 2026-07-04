#pragma once


#include <GLAD/glad.h>

namespace Engine::ECS::Component::Graphics
{
	struct ShaderComponent
	{
		Engine::Graphics::Shader* shader;
	};
}

