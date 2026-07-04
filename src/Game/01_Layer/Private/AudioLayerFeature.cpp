#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"
#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/IO/Public/FileSystemAPI.hpp"

namespace Game::Layer
{
	void AudioLayerFeature::Update(::Engine::WorldSystem::Core::WorldCtx& ctx, ::Engine::Audio::AudioSystem& audioSys)
	{
		// TODO: 記述位置を修正する必要あり
		auto& cmds = ctx.ww.GetResource<::Engine::Audio::AudioCmdBufferResource>();
		const auto& catalog = ctx.rw.GetResource<::Engine::Audio::AudioCatalogResource>();

		const auto& dt = ctx.rw.GetResource<::Engine::Time::WorldClockData>();
		const auto& mount = ctx.rw.GetResource<::Engine::IO::MountTable>();
		Engine::IO::VfsResolver resolver(mount);

		audioSys.update(catalog, cmds, resolver, dt.frameDt);
	}

}