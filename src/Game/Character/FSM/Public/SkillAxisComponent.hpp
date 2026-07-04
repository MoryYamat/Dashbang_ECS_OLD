#pragma once
#include "Engine/FSM/PUblic/FSMFwd.hpp"
#include "Engine/FSM/Public/Core/AxisComponent.hpp"

#include <cstdint>

namespace Game::Character::FSM::Skill
{ 
	enum class Field : std::uint16_t
	{
		// Triggered = 0,
		// ElapsedTime, // TimeElapsedの制御はFSM外で行う
		Triggered = 0,
		TimeElapsed = 1,
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
		// case Field::ElapsedTime: return "ElapsedTime";
		// case Field::Triggered: return "Triggered";
		default: return "";
		}
	}

	// 
	struct SkillFieldReader : Engine::FSM::Core::IFieldReader {
		// float ElapsedTime = 0.f;
		float getF32(std::uint16_t fieldIndex) const override {
			switch (static_cast<Field>(fieldIndex)) {
			// case Field::ElapsedTime: return ElapsedTime;
			default: return 0.f;
			}
		}
	};

	struct SkillTag {};
	using SkillAxisComp = Engine::FSM::Core::AxisComponent<SkillTag, SkillFieldReader>;

	// Tag化 可能
	struct SkillStateComp
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
}

namespace Engine::FSM::Core
{
	template<>
	struct AxisTraits<Game::Character::FSM::Skill::SkillTag>
	{
		static std::string_view AxisName() { return "Skill"; }
		static std::string_view DefaultFSMName() { return "Basic"; }

	};
}