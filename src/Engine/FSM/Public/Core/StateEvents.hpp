#pragma once

#include "Engine/FSM/Public/FSMFwd.hpp"

#include <cstdint>

namespace Engine::FSM::Core
{	
	// トリガ種別
	enum class StateTriggerKind : std::uint8_t
	{
		WhileIn,
		OnEnter,
		OnExit,
	};
	struct StateEventView
	{
		StateID prevState;
		StateID curState;
		ProfileID prevProf;
		ProfileID curProf;
		bool changed;

		StateEventView(StateID prevS, StateID curS, 
			ProfileID prevP, ProfileID curP, bool changed) :
			prevState(prevS), curState(curS), prevProf(prevP), curProf(curP), changed(changed) { };

		// チャタリング防止必要かも?
		bool whileIn(StateID s) const noexcept
		{
			return curState == s;
		}

		bool onEnter(StateID s) const noexcept
		{
			return changed && curState == s;
		}

		bool onExit(StateID s) const noexcept
		{
			return changed && prevState == s;
		}

		// 状態＋プロファイル
		bool whileIn(StateID s, ProfileID p) const noexcept
		{
			return curState == s && curProf == p;
		}

		bool onEnter(StateID s, ProfileID p) const noexcept
		{
			return changed && curState == s && curProf == p;
		}

		bool onExit(StateID s, ProfileID p) const noexcept
		{
			return changed && prevState == s && prevProf == p;
		}


		bool match(StateID s, ProfileID p, StateTriggerKind k) const noexcept
		{
			switch (k)
			{
			case StateTriggerKind::WhileIn:
				return whileIn(s, p);
			case StateTriggerKind::OnEnter:
				return onEnter(s, p);
			case StateTriggerKind::OnExit:
				return onExit(s, p);
			}
			return false;
		}

	};



	//template<typename ID>
	//struct EventView
	//{
	//	ID prev;
	//	ID cur;
	//	bool changed;

	//	bool whileIn(ID v) const noexcept
	//	{
	//		return cur == v;
	//	};

	//	bool onEnter(ID v) const noexcept
	//	{
	//		return changed && cur == v;
	//	};

	//	bool onExit(ID v) const noexcept
	//	{
	//		return changed && prev == v;
	//	};
	//};

	//using StateEventView = EventView<StateID>;
	//using ProfileEventView = EventView<ProfileID>;
}