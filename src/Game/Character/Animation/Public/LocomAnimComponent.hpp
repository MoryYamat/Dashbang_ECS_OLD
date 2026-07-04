#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Game::Character::Animation
{
	struct AnimationProfileComponent
	{
		std::string profileId;// 例: "PaladinDefault", "GoblinA"
	};

	struct FinalAnimDecisionComponent
	{
		bool valid = false;
		std::string clipKey;
		bool loop = true;
		float playRate = 1.0f;

		float starTime = 0.f;
		float endTime = -1.f;		// < 0 は 「クリップ末尾」
		bool loopWithinRange = false;

		// 将来は重みや優先度などを増やす

	};
}

namespace Game::Character::Animation::Movement
{
	// movement
	enum class MoveAnimType
	{
		None,
		Idle,
		RunFwd, RunBack, RunLeft, RunRight,
		RunFwdLeft, RunFwdRight,
		RunBackLeft, RunBackRight
	};

	struct MovementAnimDecisionComponent
	{
		MoveAnimType type = MoveAnimType::None;

		std::string clipKey;
		bool loop = true;
		float playRate = 1.0f;
		bool valid = false;

		// 簡易ヒステリシス用
		// 最適化対象
		MoveAnimType pending = MoveAnimType::None;
		int pendingFrames = 0;
	};

	struct LocomotionAnimQueryComponent
	{
		// movement
		bool isMoving = false;
		float moveSpeed = 0.0f;
		float normalizedSpeed = 0.0f;
		glm::vec2 moveDirWorld = { 0,1 };
		glm::vec2 facingDirWorld = { 0,1 };
		float moveYawLocal = 0.0f;// facing基準の角度 [-pi, pi]
		bool isLocomotionActive = false;

		// 
		bool valid = false;
	};
}