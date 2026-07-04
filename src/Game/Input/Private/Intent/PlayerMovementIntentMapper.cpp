#include "Game/Input/Public/InputApi.hpp"



#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"




#include "Engine/Config/CanonicalDefaults.h"



//
#include "Engine/Component/Private/Input/InputBindingComponent.hpp"
#include "Engine/Component/Private/Input/AnalogInputComponent.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/Input/Public/InputActionComponent.hpp"
#include "Game/Character/Control/Public/IntentComponent.hpp"
#include "Game/ECS/Public/CharacterAttribTags.h"
//
#include <glm/glm.hpp>



namespace Game::Input::Intent
{
	using namespace Engine::WorldSystem;
	using namespace Game::Character::Control;

	static void updatePlayerMovementIntent(
		Game::Character::Control::MovingIntentComponent& intent, 
		Game::Input::InputActionComponent& input)
	{
		glm::vec2 moveDir(0.0f);
		if (input.isPressed(InputAction::MoveForward))	moveDir.y -= 1.0f;
		if (input.isPressed(InputAction::MoveBackward))	moveDir.y += 1.0f;
		if (input.isPressed(InputAction::MoveRight))	moveDir.x += 1.0f;
		if (input.isPressed(InputAction::MoveLeft))		moveDir.x -= 1.0f;

		// std::cout << "[MovementIntentMappingSystem] moveDir = ("
		// 	<< moveDir.x << ", " << moveDir.y << ")\n";


		if (glm::length(moveDir) > 0.001f)
		{
			intent.direction = glm::normalize(moveDir);
			intent.isActive = true;
		}
		else
		{
			intent.direction = glm::vec2(0.0f);
			intent.isActive = false;
		}
		
		

	}

	static void updatePlayerDirectionIntent(
		Engine::Component::AnalogInputComponent& analog,
		Game::Character::Control::FacingIntentComponent& facing,
		Engine::Component::Logic2DTransformComponent& logicOfControllingEntity
	)
	{
		glm::vec2 dir = analog.cursorLogicPositionXZ - logicOfControllingEntity.positionXZ;

		if (glm::length(dir) > 0.0001f && !glm::any(glm::isnan(dir)))
			facing.front = glm::normalize(dir);
		else
			facing.front = CanonicalDefaults::kCanonicalForwardXZ;


		// Engine::Debug::Logging::LogVector("PlayerIntentMapper.cpp(direction)", facing.front);
	}

	void MovementIntentMappingSystem::UpdatePlayerMovementIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = Query::ViewWhere(ctx.rw, Query::All<Game::Character::Control::MovingIntentComponent,
			Game::Input::InputActionComponent, Game::ECS::Tags::PlayerCharacterTag>{});

		for (const auto& e : ents)
		{
			auto& input = ctx.ww.Get<InputActionComponent>(e);
			auto& intent = ctx.ww.Get<Game::Character::Control::MovingIntentComponent>(e);

			updatePlayerMovementIntent(intent, input);
		}

		auto ent = Query::ViewWhere(ctx.rw, Query::All<
			Engine::Component::AnalogInputComponent,
			Engine::Component::InputBindingComponent>{});

		for (const auto& e : ent)
		{
			auto& analog = ctx.ww.Get<Engine::Component::AnalogInputComponent>(e);
			auto& binding = ctx.ww.Get<Engine::Component::InputBindingComponent>(e);

			auto& facing = ctx.ww.Get<Game::Character::Control::FacingIntentComponent>(binding.controllingEntity);
			auto& playerLogic = ctx.ww.Get<Engine::Component::Logic2DTransformComponent>(binding.controllingEntity);

			updatePlayerDirectionIntent(analog, facing, playerLogic);
		}
	}
}