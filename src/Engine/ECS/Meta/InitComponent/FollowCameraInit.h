// Special template for initializing FollowCameraComponent
// FollowCameraComponent初期化用の特殊テンプレート

#pragma once

#include "Engine/ECS/Meta/InitComponent/InitSystem.h"
#include "Engine/ECS/EntityUtils/EntityUtils.h"

// #include "Core/Window/WindowManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"

// tags
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"





#include <iostream>

// 
// 廃止: 削除予定
// 実質的なカメラ初期化用システム
// TODO: カメラの微調整のために完成間近に詳細を詰める必要あり
namespace Engine::ECS::Meta::Init
{
	// 名前空間をゲーム側にする必要あり
	template<>
	struct InitSystem<Engine::ECS::Component::Camera::FollowCameraComponent>
	{
		static void Init(Engine::ECS::Component::Camera::FollowCameraComponent& followCamComp, Engine::ECS::EntityMgr& ecs, Engine::ECS::Entity cameraEntity, Engine::Window::Window& window)
		{
			Entity ePlayerActor = EntityUtils::getEntityByName(ecs, "Player");
			if (ePlayerActor == Entity::INVALID)
			{
				std::cerr << "[FollowCameraInit.cpp]: Failed to find Player entity by name!" << std::endl;
				return;
			}

			followCamComp.targetEntity = ePlayerActor;
			// Relative coordinates
			followCamComp.offset = { 0.0f, 7.0f,5.0f };
			//followCamComp.offset = { 0.0f, 30.0f,20.0f };

			// get 
			Engine::ECS::Component::Common::TransformComponent& targetTransform = ecs.get<Engine::ECS::Component::Common::TransformComponent>(ePlayerActor);
			Engine::ECS::Component::Logic2D::Logic2DTransformComponent& targetLogic2DTransform = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(ePlayerActor);

			// Initialize camera position
			auto& camTransform = ecs.get<Engine::ECS::Component::Common::TransformComponent>(cameraEntity);
			camTransform.position = targetTransform.position + followCamComp.offset;

			// カギは frontOffset と バネ

			// Initialize camera Front vector
			auto& camComp = ecs.get<Engine::ECS::Component::Camera::CameraComponent>(cameraEntity);
			camComp.fov = glm::radians(45.0f);
			camComp.aspect = window.GetAspect();
			camComp.frontOffset = glm::vec3(0.0f, 0.0f, -0.6f);
			camComp.front = glm::normalize(targetTransform.position - camTransform.position + camComp.frontOffset);
			camComp.right = glm::normalize(glm::cross(camComp.front, camComp.up));



			Engine::Debug::Logging::LogVector("FollowCameraInit.h", camComp.front);

			Engine::Debug::Logging::GeneralLog("FollowCameraInit.h", "FollowCamera");
		}
	};
}

