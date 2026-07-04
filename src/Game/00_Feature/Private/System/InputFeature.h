//

#pragma once

#include "Engine/InputManager/Public/InputFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"

#include "Engine/ECS/EntityManager.h"

// 廃止：削除予定
namespace Game::Feature::Setting
{
}

namespace Game::Feature::Input
{
	class InputFeature
	{
	public:

		static void InitInputMapping(Engine::WorldSystem::Core::WorldCtx& ctx);
		static void InputRoutingSystem(Engine::WorldSystem::Core::WorldCtx& ctx, const Engine::Input::RawInputState& raw);

		static void AnalogInputRoutingSystem(Engine::WorldSystem::Core::WorldCtx& ctx,
			const Engine::Input::RawInputState& rawInput, const Engine::Graphics::RenderContext& renderContext);
	};
}