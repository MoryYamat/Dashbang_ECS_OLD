#pragma once
#include "Engine/FSM/PUblic/FSMFwd.hpp"
#include "Engine/FSM/Public/Core/AxisComponent.hpp"

#include <cstdint>

namespace Game::Character::FSM::Movement
{
	enum class Field : std::uint16_t
	{
		MovementInputMag = 0,
		Count
	};

	constexpr std::uint16_t to_index(Field f)
	{
		return static_cast<std::uint16_t>(f);
	}

	// 逆引きやデバッグ用
	constexpr std::string_view to_name(Field f)
	{
		switch (f)
		{
		case Field::MovementInputMag: return "movementInputMag";
		default: return "";
		}
	}

	struct MovementFieldReader : Engine::FSM::Core::IFieldReader {
		float movementInputMag = 0.f;
		float getF32(std::uint16_t fieldIndex) const override {
			switch (static_cast<Field>(fieldIndex)) {
			case Field::MovementInputMag: return movementInputMag;
			default: return 0.f;
			}
		}
	};

	struct MovementTag{};
	using MovementAxisComp = Engine::FSM::Core::AxisComponent<MovementTag, MovementFieldReader>;

	struct MovementStateComp
	{
		Engine::FSM::Core::StateID curState = Engine::FSM::Core::kInvalidState;		// 軸宇宙のID StateID.v
		Engine::FSM::Core::StateID prevState = Engine::FSM::Core::kInvalidState;	// 軸宇宙のID StateID.v

		Engine::FSM::Core::ProfileID curProf = Engine::FSM::Core::kInvalidProfile;	// 軸宇宙のID Profile.v
		Engine::FSM::Core::ProfileID prevProf = Engine::FSM::Core::kInvalidProfile;

		bool changedThisFrame = false;


		void Transition(Engine::FSM::Core::StateID from, Engine::FSM::Core::StateID to)
		{
			curState = to;
			prevState = from;
			changedThisFrame = true;
		}

		void Transition(Engine::FSM::Core::StateID from, Engine::FSM::Core::StateID to,
						Engine::FSM::Core::ProfileID pfrom, Engine::FSM::Core::ProfileID pto)
		{
			curState = to;
			prevState = from;
			curProf = pto;
			prevProf = pfrom;
			changedThisFrame = true;
		}

		void ResetChanged()
		{
			changedThisFrame = false;
		}
	};

	struct MovementModifierEntry
	{
		float mul = 1.0f;		// 速度倍率 (0.5 → 半減、2.0 → 2倍)
		float add = 0.0f;		// 必要なら加算分
		float remaining = 0.0f;	// 残り時間
	};

	struct MovementModifierComponent
	{
		std::vector<MovementModifierEntry> entries;
	};
}

namespace Engine::FSM::Core
{
	template<>
	struct AxisTraits<Game::Character::FSM::Movement::MovementTag>
	{
		static std::string_view AxisName() { return "Movement"; }
		static std::string_view DefaultFSMName() { return "Basic"; }

	};
}