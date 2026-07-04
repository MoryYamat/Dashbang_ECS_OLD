#include "Game/FSM/Public/FSMApi.hpp"
// #include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/FSMApi.hpp"
#include "Game/FSM/Private/InitFunctions.hpp"
#include "Game/Character/FSM/Public/FSMApi.hpp"
#include "Game/Character/FSM/Public/MovementAxisApi.hpp"

#include "Game/Character/FSM/Public/SkillAxisApi.hpp"

namespace Game::FSM
{
	// using namespace Engine::FSM::Core;

	using namespace Engine::FSM::Core;

	bool InitAllFSMs_Game(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		//auto registerFn = [](FSMRegistry& reg)
		//	{
		//		Game::Character::FSM::Movement::RegisterMovementAxes(reg);
		//		Game::Character::FSM::Skill::RegisterSkillAxes(reg);
		//	};


		auto registerFn = MakeGameRegisterProvider();

		auto resolverProvider = MakeGameResolverProvider();

		if(!InitAllFSMs(ctx, registerFn, resolverProvider, BuildStrictness::Strict))
			return false;

		if (!InitGameTable(ctx))
			return false;


		return true;
	}

}