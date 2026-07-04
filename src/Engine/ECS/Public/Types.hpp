#pragma once

#include "Engine/Window/Public/WindowFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/ECS/Public/Entity.hpp"

#include "Engine/Component/Private/Camera/FollowCameraComponent.hpp"

#include <cstdint>

namespace Engine::ECS::Core
{
	namespace Init
	{
		enum class InitPhase : std::uint8_t
		{
			immediate,
			deffered,
		};

		// 特殊化用関数 (遅らせ初期化が必要なComponentについて特殊化テンプレートを`deffered`で定義する
		template<typename Component>
		constexpr InitPhase GetInitPhase()
		{
			return InitPhase::immediate;
		}

		template<typename Component>
		struct InitSystem
		{
		public:
			static void Init(Component& comp, Engine::ECS::Core::Entity e,
				Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Window::Window& window)
			{
				// 特化して使うのでここでは不要
			}
		private:
		};

		template<>
		constexpr InitPhase GetInitPhase<Engine::Component::FollowCameraComponent>()
		{
			return InitPhase::deffered;
		};
	};
}