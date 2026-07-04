#include "Game/Input/Public/InputApi.hpp"



#include "Engine/InputManager/Public/Types.hpp"





#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Input/AnalogInputComponent.hpp"
#include "Engine/Graphics/Public/Types.hpp"

#include "Game/Utils/Public/GameUtilsApi.hpp"

// 削除予定


#include <cassert>

namespace Game::Input
{
	void RouteAnalogInput(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Input::RawInputState& rawInput, const Engine::Graphics::RenderContext& renderContext)
	{
		auto ents = Engine::WorldSystem::Query::ViewWhere(ctx.rw, Engine::WorldSystem::Query::All<Engine::Component::AnalogInputComponent>{});

		for(const auto& e : ents)
		{
			auto& analog = ctx.ww.Get<Engine::Component::AnalogInputComponent>(e);

			analog.cursorLogicPositionXZ = Game::Utils::ProjectScreenToLogicXZPlane(rawInput.mousePosition, renderContext);
			analog.cursorDelta = rawInput.mouseDelta;
			analog.scrollDelta = rawInput.scrollDelta;
		};
	}
}