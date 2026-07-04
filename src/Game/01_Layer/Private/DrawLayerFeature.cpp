#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Window/Public/Window.h"
#include "Engine/Graphics/Public/Types.hpp"
#include "Engine/Graphics/Public/GraphicsApi.hpp"

#include "Game/Camera/Public/CameraApi.hpp"

#include "Game/00_Feature/Public/EffectFeature.hpp"

namespace Game::Layer
{
	void DrawLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader& shader
		, Engine::Window::Window& window, Engine::Graphics::RenderContext& renderCtx)
	{

		// camera
		Game::Camera::Update(ctx);


		// renderer
		Engine::Graphics::UpdateRendererAll(ctx, shader, window.GetAspect(), renderCtx);
	}


	void DrawLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader& shader, ::Engine::Graphics::RendererResources& res,
		::Engine::Window::Window& window, ::Engine::Graphics::RenderContext& renderCtx)
	{
		Game::Camera::Update(ctx);

		Engine::Graphics::UpdateRendererAll(ctx, shader, res, window.GetAspect(), renderCtx);

		::Game::Feature::VFX::VFXFeature::Update(ctx);
	}
}