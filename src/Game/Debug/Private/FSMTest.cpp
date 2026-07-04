#include "Game/Debug/Public/DebugApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/Control/Public/IntentComponent.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Game/Character/FSM/Public/MovementAxisApi.hpp"

#include "Engine/Component/Private/Tags/DummyTagComp.hpp"

#include <iostream>

namespace Game::Debug::FSM
{
	using namespace ::Engine::WorldSystem::Core;
	using namespace ::Engine::WorldSystem::Query;
	namespace Ctrl = Game::Character::Control;
	namespace MFSM = Game::Character::FSM::Movement;
	using namespace Game::Character::FSM;


	void TestFSMDummyDriverSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		static std::uint64_t frame = 0;
		frame++;

		const bool moveOn = (frame % 120) < 60; // 60フレームON, 60フレームOFF


		const auto dummies = ViewWhere(ctx.rw, All<::Engine::Component::DummyTag, Ctrl::MovingIntentComponent, Ctrl::FacingIntentComponent, MFSM::MovementStateComp>{});

		for (auto& e : dummies)
		{
			auto& mv = ctx.ww.Get<Ctrl::MovingIntentComponent>(e);
			mv.isActive = moveOn;
			mv.direction = moveOn ? glm::vec2{ 1.0f, 0.0f } : glm::vec2{ 0.0f, 0.0f };

		}
	}
}