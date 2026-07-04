#pragma once

namespace Engine::Component
{
	//Camera
	struct CameraComponent;
	struct FollowCameraComponent;

	//Common
	struct TransformComponent;

	// Graphics
	struct AnimatorComponent;
	struct MaterialComponent;
	struct MeshComponent;
	struct ModelSizeComponent;
	struct ShaderComponent;

	// Input
	struct AnalogInputComponent;
	struct InputBindingComponent;
	struct MouseCursorComponent;

	//Logic2D
	struct CollisionComponent;
	struct Logic2DTransformComponent;
	struct TileGridXZComponent;
	struct TileMapComponent;
	struct TileSpaceComponent;
	struct Transform2DComponent;
	struct Velocity2DComponent;

	// Tags
	struct ObstacleTagComponent;
	struct PendingDestroyComponent;
	struct PlayerControllerComponent;
	struct TileMapTag;

	//Utils
	struct NameComponent;
	struct ParentComponent;
}