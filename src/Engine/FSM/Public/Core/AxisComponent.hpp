#pragma once

#include "Engine/FSM/Public/Core/AxisInstance.hpp"


#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include <cstdint>
#include <string_view>


namespace Engine::FSM::Core
{
	template<typename AxisTag, typename ReaderT>
	struct AxisComponent
	{
		Engine::FSM::Core::AxisInstance inst;	// AxisInstance
		ReaderT reader;							//	軸固有の入力集約先
		std::uint32_t initState = 0;
		std::uint32_t initProfile = 0;
	};

	template<typename AxisTag>
	struct AxisTraits
	{
		// ゲーム側で特殊化
		static std::string_view AxisName();				//例: "Movement"
		static std::string_view DefaultFSMName();		//例: "Basic"
	};
}