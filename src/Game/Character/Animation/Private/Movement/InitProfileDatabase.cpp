
#include "AnimationProfile.hpp"


#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Character::Animation::Movement
{

	bool InitMoveAnimProfile(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& db = ctx.ww.CreateResource<MovementAnimationProfileDatabase>();

		bool ok = false;
		// TODO: データ駆動化
		db.movements["PaladinDefault"] = Movement::MovementAnimationProfile
		{
			.idle = "idle_default",
			.runFwd = "run_fwd_default",
			.runBack = "run_back_default",
			.runRight = "run_right_default",
			.runLeft = "run_left_default",
			.runFwdLeft = "run_fwd_left_default",
			.runFwdRight = "run_fwd_right_default",
			.runBackLeft = "run_back_left_default",
			.runBackRight = "run_back_right_default",
		};

		ok = true;
		return ok;
	}
}