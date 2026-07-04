#include "Game/Character/Animation/Private/InternalApi.hpp"

#include "Game/Character/Animation/Public/LocomAnimComponent.hpp"
#include "Game/Character/Animation/Private/Movement/AnimationProfile.hpp"

#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Game/ECS/Public/CharacterAttribTags.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace Game::Character::Animation::Movement
{
	constexpr float kEps = 1e-6f;

	using namespace Engine::Component;
	using namespace Engine::WorldSystem::Query;
	
	void LocomQuerySystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = ViewWhere(ctx.rw, All<Logic2DTransformComponent, Velocity2DComponent, 
			 Game::Character::FSM::Movement::MovementStateComp, LocomotionAnimQueryComponent>{});

		for (const auto& e : ents)
		{
			auto& q = ctx.ww.Get<LocomotionAnimQueryComponent>(e);
			const auto& state = ctx.ww.Get<Game::Character::FSM::Movement::MovementStateComp>(e);

			const auto& ts = ctx.ww.Get<Logic2DTransformComponent>(e);
			const auto& vel = ctx.ww.Get<Velocity2DComponent>(e);

			const glm::vec2 facing = ts.front;
			const bool isMoving = (glm::length2(vel.velocity) > kEps);
			const glm::vec2 moveDir = isMoving ? glm::normalize(vel.velocity) : facing;

			q.isMoving = isMoving;
			q.facingDirWorld = facing;
			q.moveDirWorld = moveDir;

			q.valid = true;

		}
	}

	static MoveAnimType calcRelativeMovementDir(const glm::vec2 facingDir, const glm::vec2 movingDir,
		const Game::Character::Animation::Movement::MoveAnimType prev)
	{
		using Type = MoveAnimType;
		namespace Math2D = Engine::Math::Logic2D;

		constexpr float k45 = glm::radians(45.0f);
		constexpr float k135 = glm::radians(135.0f);
		constexpr float kHys = glm::radians(5.0f);

		const float k45on = k45 + kHys;
		const float k45off = k45 - kHys;
		const float k135on = k135 + kHys;
		const float k135off = k135 - kHys;

		glm::vec2 f = (Math2D::Vector::SquaredLength(facingDir)) > 0.f ? glm::normalize(facingDir) : glm::vec2(0, 1);
		glm::vec2 m = (Math2D::Vector::SquaredLength(movingDir)) > 0.f ? glm::normalize(movingDir) : f;

		// +Z 軸 基準 [0, 2pi)
		const float yawF = Math2D::CalcYawFromDirection(f);
		const float yawM = Math2D::CalcYawFromDirection(m);

		// const float yaw = Math2D::ToSignedPi(yawM - yawF); // [-pi, pi]// もし逆にしたいならこちらを使う
		const float yaw = Math2D::ToSignedPi(yawF - yawM); // [-pi, pi]
		const float ay = std::abs(yaw);


		//  TODO: switchはできるだけ避けたい->今後ほかの良い方法を考える
		// 最適化必要
		//switch(prev)
		//{
		//case Type::RunFwd:
		//	if (ay <= k45off) return Type::RunFwd;// 前を維持
		//	break;
		//case Type::RunBack:
		//	if (ay >= k135off) return Type::RunBack;// 後ろを維持
		//	break;
		//case Type::RunLeft:
		//	if (ay <= k45off) return Type::RunFwd;
		//	if (ay >= k135on) return Type::RunBack;
		//	if (yaw > 0.0f) return Type::RunLeft;

		//	return Type::RunRight;

		//case Type::RunRight:
		//	if (ay <= k45off) return Type::RunFwd;
		//	if (ay >= k135on) return Type::RunBack;
		//	if (yaw < 0.0f) return Type::RunRight;

		//	return Type::RunLeft;

		//default:
		//	break;
		//}

		float rad = yaw;
		if (rad < 0) rad += glm::two_pi<float>();


		// 8方向分割 (22.5°, 45°, 67.5° ... に対応)
		constexpr float k22_5 = glm::radians(22.5f);
		constexpr float k67_5 = glm::radians(67.5f);
		constexpr float k112_5 = glm::radians(112.5f);
		constexpr float k157_5 = glm::radians(157.5f);
		constexpr float k202_5 = glm::radians(202.5f);
		constexpr float k247_5 = glm::radians(247.5f);
		constexpr float k292_5 = glm::radians(292.5f);
		constexpr float k337_5 = glm::radians(337.5f);

		if (rad < k22_5 || rad >= k337_5) return Type::RunFwd;
		if (rad < k67_5)  return Type::RunFwdRight;
		if (rad < k112_5) return Type::RunRight;
		if (rad < k157_5) return Type::RunBackRight;
		if (rad < k202_5) return Type::RunBack;
		if (rad < k247_5) return Type::RunBackLeft;
		if (rad < k292_5) return Type::RunLeft;
		return Type::RunFwdLeft;



		// 通常判定
		if (ay < k45on) return Type::RunFwd;
		else if (ay > k135on) return Type::RunBack;
		else if (yaw < 0.0f) return Type::RunRight;
		else return Type::RunLeft;

		// fwd

		// back

		// righ 

		// left
	};

		// TODO: 最適化
	// TODO: 最適化
	// TODO: 最適化
	static MoveAnimType quantizeTo4Dir(const MoveAnimType type)
	{
		using Type = MoveAnimType;

		switch (type)
		{
		case Type::RunFwdLeft:
		case Type::RunFwdRight:
			return Type::RunFwd;

		case Type::RunBackLeft:
		case Type::RunBackRight:
			return Type::RunBack;

		default:
			return type; // それ以外はそのまま（Idle, RunLeft, RunRight）
		}
	};

	void MovementAnimationResolver::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		using Type = MoveAnimType;
		auto& db = ctx.ww.GetResource<MovementAnimationProfileDatabase>();


		auto ents = ViewWhere(ctx.rw, All<AnimationProfileComponent
			, MovementAnimDecisionComponent, LocomotionAnimQueryComponent>{});


		for (const auto& e : ents)
		{
			const auto& q = ctx.ww.Get<LocomotionAnimQueryComponent>(e);
			const auto& prof = ctx.ww.Get<AnimationProfileComponent>(e);
			auto& dec = ctx.ww.Get<MovementAnimDecisionComponent>(e);


			if (!q.valid)
			{
				dec.valid = false;
				continue;
			}

			const bool isPlayer = ctx.rw.Has<Game::ECS::Tags::PlayerCharacterTag>(e);
			const MoveAnimType want = q.isMoving ? 
				calcRelativeMovementDir(q.facingDirWorld, q.moveDirWorld, dec.type) : MoveAnimType::Idle;

			const auto* mp = db.FindMovement(prof.profileId);
			assert(mp && "Could not get movement animation profile.");

			if (!isPlayer)
			{
				dec.type = quantizeTo4Dir(want);
			}
			else
			{
				if (want == dec.type)
				{
					// 同じ方向ならリセット
					dec.pendingFrames = 0;
					dec.pending = want;
				}
				else
				{
					if (dec.pending != want)
					{
						// 新しい候補が出たのでリセット
						dec.pending = want;
						dec.pendingFrames = 0;
					}
					else
					{
						// 同じ方向候補が続いている
						if (++dec.pendingFrames >= 3) // 閾値
						{
							dec.type = dec.pending;
							dec.pendingFrames = 0;
						}
					}
				}
			}

			// TODO: データ駆動化
			switch (dec.type)
			{
			case Type::Idle: dec.clipKey = mp->idle.empty() ? "idle_default" : mp->idle; break;
			case Type::RunFwd: dec.clipKey = mp->runFwd.empty() ? "run_fwd_default" : mp->runFwd; break;
			case Type::RunBack: dec.clipKey = mp->runBack.empty() ? "run_back_default" : mp->runBack; break;
			case Type::RunRight: dec.clipKey = mp->runRight.empty() ? "run_right_default" : mp->runRight; break;
			case Type::RunLeft: dec.clipKey = mp->runLeft.empty() ? "run_left_default" : mp->runLeft; break;
			case Type::RunFwdLeft:   dec.clipKey = mp->runFwdLeft.empty() ? "run_fwd_left_default" : mp->runFwdLeft;  break;
			case Type::RunFwdRight:  dec.clipKey = mp->runFwdRight.empty() ? "run_fwd_right_default" : mp->runFwdRight; break;
			case Type::RunBackLeft:  dec.clipKey = mp->runBackLeft.empty() ? "run_back_left_default" : mp->runBackLeft; break;
			case Type::RunBackRight: dec.clipKey = mp->runBackRight.empty() ? "run_back_right_default" : mp->runBackRight;break;
			}

			dec.loop = true;
			dec.playRate = 1.0f;
			dec.valid = true;

			// std::cout << dec.clipKey.c_str() << "\n";
		}
	}
}