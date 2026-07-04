#include "Game/Audio/Public/GameAudioAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"

#include "Game/Audio/Internal/GameAudioLog.hpp"
#include "Game/Audio/Internal/GameAudioInternalAPI.hpp"
#include "Game/Audio/Private/CatalogBuilder.hpp"


namespace Game::Audio
{
	using	namespace Engine::Audio;
	bool InitAllGameAudioSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& ids = ctx.ww.CreateResource<AudioIds>();

		auto* const catalog = ctx.ww.TryGetResource<AudioCatalogResource>();

		if (!catalog)
		{
			Log::error("Failed to get a pointer to AudioCatalogResource or AudioBufferResource (may not be registered in worldResource).");
			return false;
		}

		auto& cat = *catalog;

		AudioCatalogBuilder builder{};

		RegisterAudioDTO(builder);
		builder.Build(cat, ids);
		if (!builder.Ok())
		{
			Log::info("There is an error in the build result of AudioCatalogBuilder");
		}

		return true;
	}
}
