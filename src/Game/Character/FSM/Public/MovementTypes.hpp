// Movement Axis DTO

#pragma once

#include "Engine/Public/EngineFwd.hpp"
#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/StateEvents.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"

#include <cstdint>
#include <vector>
#include <span>

namespace Game::Character::FSM::Movement
{

	struct MovementCondTable
	{
		//Engine::FSM::Core::CondID 
		// bool changedThisFrame = false;
	};

	struct MovementStateTable
	{
		Engine::FSM::Core::StateID Idle = Engine::FSM::Core::kInvalidState;		// 軸宇宙 = StateID.v
		Engine::FSM::Core::StateID Moving = Engine::FSM::Core::kInvalidState;
		bool changedThisFrame = false;
	};

	struct MovementProfileTable
	{
		Engine::FSM::Core::ProfileID Default = Engine::FSM::Core::kInvalidProfile;// 軸宇宙
	};

	struct MovementPipelineEntry
	{
		Engine::ECS::Core::Entity e;

		MovementAxisComp* axis;
		MovementStateComp* state;
		Engine::Component::Velocity2DComponent*  vel;

	};
	using MovementPipeline = std::vector<MovementPipelineEntry>;

	// 副作用(制御対象)の種類
	enum class MovementOpKind : std::uint8_t
	{
		ApplyVelocityFromIntent = 0,	// 入力から速度を決める
		ZeroVelocity = 1,				// 停止
		// 追加可能
		// playWalkAnim,
		// playIdelAnim,
		// SpawnDustEffect,
		COUNT
	};

	inline constexpr std::uint32_t OpBit(MovementOpKind k) noexcept
	{
		return 1u << static_cast<std::uint32_t>(k);
	}

	struct MovementLogicKey
	{
		Engine::FSM::Core::StateID state;
		Engine::FSM::Core::ProfileID profile;
		Engine::FSM::Core::StateTriggerKind trigger;
	};

	struct MovementLogicEntry
	{
		MovementLogicKey key;
		std::uint32_t opMask = 0;	// MovementOpKind を ビットで詰める
	};

	struct MovementLogicTable
	{
		std::vector<MovementLogicEntry> entries;
	};

	struct MovementFSMSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<MovementPipelineEntry> ents, const float dt);
	};

	struct MovementEnvSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<MovementPipelineEntry> ents, const float dt);
	};

	struct MovementLogicSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(std::span<MovementPipelineEntry> ents, const float dt);
	};
}
