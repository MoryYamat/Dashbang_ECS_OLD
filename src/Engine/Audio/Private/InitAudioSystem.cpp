#include "Engine/Audio/Public/AudioAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


namespace Engine::Audio
{
	void RegisterAudioResources(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& catalog = ctx.ww.CreateResource<AudioCatalogResource>();
		auto& cmds = ctx.ww.CreateResource<AudioCmdBufferResource>();

	}

	void InitAllAudioResourceSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		RegisterAudioResources(ctx);
	}
}