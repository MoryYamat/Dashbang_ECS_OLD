// Rendering system

#pragma once

// Engine
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Camera/CameraComponent.h"

#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

#include "Engine/Graphics/Private/Renderer/Shader.h"
#include "Engine/Graphics/Private/Renderer/RenderContext.h"



namespace Engine::Graphics::Render
{
	// void RenderSystem(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader& shader, float aspect);
	// void RenderSystem(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader& shader, float aspect, RenderContext& context);

	// 現在未使用
	// void UpdateRenderContext(Engine::ECS::EntityMgr& ecs, Engine::Graphics::RenderContext& context);

	//void drawMesh(const Engine::ECS::Component::Graphics::MeshComponent& meshComp);


	//bool getCameraMatrices(Engine::ECS::EntityMgr& ecs, glm::mat4& view, glm::mat4& projection);
	//bool getCameraMatrices(Engine::ECS::EntityMgr& ecs, glm::mat4& view, glm::mat4& projection, RenderContext& context);

	//glm::mat4 computeViewMatrix(const Engine::ECS::Component::Common::TransformComponent& transformComp, const Engine::ECS::Component::Camera::CameraComponent& cameraComp);


	//glm::mat4 computeViewMatrix(const Engine::ECS::Component::Common::TransformComponent& transformComp, const Engine::ECS::Component::Camera::CameraComponent& cameraComp);

	//glm::mat4 computeProjectionMatrix(float fov, float aspect, float nearClip, float farClip);
}

