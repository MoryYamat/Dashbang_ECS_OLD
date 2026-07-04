#pragma once

#include "Engine/ECS/Public/ECSApi.hpp"
#include "Engine/ECS/Public/Types.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/Window/Public/Window.h"
#include "Engine/WorldSystem/Private/Core/WorldCtx.hpp"

#include "Engine/Component/Private/Camera/CameraComponent.hpp"
#include "Engine/Component/Private/Camera/FollowCameraComponent.hpp"
#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"


namespace Engine::ECS::Core
{
	namespace Init
	{
		template<>
		struct InitSystem<Engine::Component::FollowCameraComponent>
		{
			static void Init(Engine::Component::FollowCameraComponent& followCamComp,
				Engine::ECS::Core::Entity cameraEntity, Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Window::Window& window)
			{
				using namespace Engine::Component;

				Entity ePlayerActor = EntityUtils::getEntityByName(ctx, "Player");
				if (ePlayerActor == INVALID)
				{
					std::cerr << "[FollowCameraInit.cpp]: Failed to find Player entity by name!" << std::endl;
					return;
				}

				// TODO: 定数で与えられるよにする
				// FollowCamera の初期化情報
				followCamComp.targetEntity = ePlayerActor;
				// Relative coordinates
				followCamComp.offset = { 0.0f, 7.0f,5.0f };
				//followCamComp.offset = { 0.0f, 30.0f,20.0f };

				// get 
				Engine::Component::TransformComponent& targetTransform = ctx.ww.Get<TransformComponent>(ePlayerActor);
				Engine::Component::Logic2DTransformComponent& targetLogic2DTransform = ctx.ww.Get<Logic2DTransformComponent>(ePlayerActor);

				// Initialize camera position
				auto& camTransform = ctx.ww.Get<TransformComponent>(cameraEntity);
				camTransform.position = targetTransform.position + followCamComp.offset;

				// カギは frontOffset と バネ

				// Initialize camera Front vector
				auto& camComp = ctx.ww.Get<CameraComponent>(cameraEntity);
				camComp.fov = glm::radians(45.0f);
				camComp.aspect = window.GetAspect();
				camComp.frontOffset = glm::vec3(0.0f, 0.0f, -0.6f);
				camComp.front = glm::normalize(targetTransform.position - camTransform.position + camComp.frontOffset);
				camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));

			}
		};
	}

}