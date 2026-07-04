#include "Game/Actor/Public/TestCamActor.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Camera/FollowCameraComponent.hpp"

#include "Engine/Component/Private/Camera/CameraComponent.hpp"
#include "Engine/Component/Private/Tags/PlayerControllerComponent.hpp"

namespace Game::Actor
{
	using namespace Engine::Component;
	using namespace Engine::ECS::Core;


	TestFollowCamActor::TestFollowCamActor(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Entity e = ctx.ww.Create();

		ctx.ww.Add<CameraComponent>(e);
		ctx.ww.Add<FollowCameraComponent>(e);
		ctx.ww.Add<TransformComponent>(e);
	}

	TestFixedCamActor::TestFixedCamActor(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Entity e = ctx.ww.Create();

		TransformComponent& trans = ctx.ww.Add<TransformComponent>(e);

		CameraComponent& cam = ctx.ww.Add<CameraComponent>(e);

		ctx.ww.Add<PlayerControllerComponent>(e);

		cam.aspect = 1280.0f / 720.f;
		cam.nearClip = 0.1f;
		cam.farClip = 100.0f;
		cam.fov = glm::radians(120.0f);
		cam.target = glm::vec3(0.0f);
		cam.up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
}