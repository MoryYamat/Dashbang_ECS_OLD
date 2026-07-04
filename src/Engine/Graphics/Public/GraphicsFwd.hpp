#pragma once

#include "Engine/Core/Public/StrongID.hpp"

namespace Engine::Graphics
{
	class Shader;
	struct RenderContext;
	struct RendererResources;
	class ITextureProvider;

	struct TextureHandleTag{};
	using TextureHandle = ::Engine::Core::StrongID<TextureHandleTag, std::uint32_t>;
}

// 以下廃止予定
namespace Engine::Graphics
{
	namespace Animation
	{
		class Animator;
		struct TRS;
	}

	namespace Model
	{
		struct TextureData;
		struct MaterialData;
		struct VertexData;
		struct Bone;
		struct Skeleton;
		enum class ChannelType;
		struct Channel;
		struct AnimationClip;
		struct MeshData;
		struct ModelData;
		struct MeshGPU;
		struct ModelGPU;
	}
}
	