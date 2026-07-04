// Camera Follow system

#pragma once
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Camera/CameraComponent.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"


// 廃止: 削除予定
// Camera Following System
namespace Game::Camera
{
	void Update(Engine::ECS::EntityMgr& ecs, float deltaTime);



	// 不要かも（カメラの位置がモデルの正面ベクトルの対角方向にくるようにカメラの位置と向きを初期化するテンプレート）
	void setFollowCameraGetFront(
		Engine::ECS::Component::Camera::FollowCameraComponent& followCamComp
		, Engine::ECS::Component::Logic2D::Logic2DTransformComponent& targetLogic2DTransform
	);
	
	// こっちは必要かも
	void setCameraVectors(
		Engine::ECS::Component::Camera::CameraComponent& camComp
		, Engine::ECS::Component::Common::TransformComponent& camTransform
		, Engine::ECS::Component::Common::TransformComponent& targetTransformComp
	);

};