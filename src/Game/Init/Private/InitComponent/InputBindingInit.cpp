#include "InputBindingInit.h"

#include "Engine/ECS/Component/Input/AnalogInputComponent.h"
#include "Engine/ECS/Component/Input/InputBindingComponent.h"

//
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Input/AnalogInputComponent.hpp"
#include "Engine/Component/Private/Input/InputBindingComponent.hpp"
#include "Engine/Component/Private/Tags/PlayerControllerComponent.hpp"
//

#include "Game/ECS/Public/CharacterAttribTags.h"


// 削除予定
void Game::Init::Input::InputBindingInitializationSystem(Engine::ECS::EntityMgr& ecs)
{
	for (Engine::ECS::Entity cursor : ecs.view<
		Engine::ECS::Component::Input::AnalogInputComponent,
		Engine::ECS::Component::Input::InputBindingComponent>())
	{
		auto& binding = ecs.get<Engine::ECS::Component::Input::InputBindingComponent>(cursor);

		for (Engine::ECS::Entity Player : ecs.view<
			Game::ECS::Tags::PlayerCharacterTag>())
		{
			binding.controllingEntity = Player;

			std::cout << "[InputBindingInit(palyer-analoginput)] palyer: " << Player.id << " binding: " << binding.controllingEntity.id << std::endl;
		}
	}
}

namespace Game::Init::Input
{
	using namespace Engine::Component;

	void InputBindingInitializationSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		ctx.ww.ForEachAlive([&](Engine::ECS::Core::Entity e) {
			auto* analog = ctx.ww.TryGet<AnalogInputComponent>(e);
			auto* bind = ctx.ww.TryGet<InputBindingComponent>(e);


			if (!analog || !bind) return;

			Engine::ECS::Core::Entity e_analog = e;

			ctx.ww.ForEachAlive([&](Engine::ECS::Core::Entity e)
				{
					auto* player = ctx.ww.TryGet<Game::ECS::Tags::PlayerCharacterTag>(e);

					if (!player) return;

					bind->controllingEntity = e;

					std::cout << "[InputBindingInit(palyer-analoginput)] palyer: " << e.id << " binding: " << e_analog.id << std::endl;
				});
			});
	}
}