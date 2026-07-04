#include "Game/Actor/Public/TestDummyActor.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/Control/Public/IntentComponent.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Game/Character/FSM/Public/MovementAxisApi.hpp"
#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"
#include "Game/Character/FSM/Public/SkillAxisApi.hpp"

#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"

#include "Engine/Component/Private/Tags/DummyTagComp.hpp"

namespace Game::Actor::Debug
{
	namespace Ctrl = Game::Character::Control;
	namespace MFSM = Game::Character::FSM::Movement;	
	using namespace Game::Character::FSM;

	TestFSMDummyActor::TestFSMDummyActor(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		const auto e = ctx.ww.Create();

		ctx.ww.Add<::Engine::Component::Velocity2DComponent>(e);

		// control
		Ctrl::MovingIntentComponent& mvint = ctx.ww.Add<Ctrl::MovingIntentComponent>(e);
		Ctrl::FacingIntentComponent& facing = ctx.ww.Add<Ctrl::FacingIntentComponent>(e);

		// fsm
		MFSM::MovementStateComp& MFSMstate = ctx.ww.Add<MFSM::MovementStateComp>(e);
		MFSM::MovementAxisComp& MFSMComp = ctx.ww.Add<MFSM::MovementAxisComp>(e);
		auto& moveMods = ctx.ww.Add<MFSM::MovementModifierComponent>(e);
		if (!MFSM::InitMovementAxis(ctx, MFSMComp))
		{
			std::cout << "[TestActor]: Failed to initialize the movement axis component.\n";
		}
		auto& sState = ctx.ww.Add<Skill::SkillStateComp>(e);
		auto& sFSM = ctx.ww.Add<Skill::SkillAxisComp>(e);
		if (!Skill::InitSkillAxis(ctx, sFSM))
		{
			std::cout << "[TestActor]: Failed to initialize the Skill axis component.\n";
		}

		ctx.ww.Add<Engine::Component::DummyTag>(e);
	}
}