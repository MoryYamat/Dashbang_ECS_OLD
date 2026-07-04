// 特定のComponent型に対して初期化タイミングを特殊化
// Specialize the initialization timing for a particular Component type.
#pragma once

#include "Engine/ECS/Meta/InitPhase.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"


namespace Engine::ECS::Meta::Init
{
	template<>// Specialize template テンプレート特殊化
	constexpr InitPhase GetInitPhase<Engine::ECS::Component::Logic2D::TileMapComponent>()
	{
		return InitPhase::Deferred;
	}

	template<>
	constexpr InitPhase GetInitPhase <Engine::ECS::Component::Camera::FollowCameraComponent > ()
	{
		return InitPhase::Deferred;
	}
}

