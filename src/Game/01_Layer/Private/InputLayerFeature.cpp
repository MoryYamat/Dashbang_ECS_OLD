#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/InputManager/Public/Types.hpp"
#include "Engine/Graphics/Private/Renderer/RenderContext.h"

#include "Game/00_Feature/Private/System/InputFeature.h"

namespace Game::Layer
{
	
	void InputLayer::Update(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Input::RawInputState& raw, const Engine::Graphics::RenderContext& renderCtx)
	{
		Game::Feature::Input::InputFeature::AnalogInputRoutingSystem(ctx, raw, renderCtx);
		Game::Feature::Input::InputFeature::InputRoutingSystem(ctx, raw);
	};
}