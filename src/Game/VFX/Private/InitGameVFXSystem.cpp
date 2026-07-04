#include "Game/VFX/Public/GameVFXAPI.hpp"

#include "Game/VFX/Internal/GameVFXInternalAPI.hpp"

#include "Game/VFX/Private/GameVFXCatalogBuilder.hpp"

#include "Game/VFX/Internal/GameVFXLog.hpp"

#include "Engine/VFX/Public/VFXAPI.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::VFX
{
	void InitAllGameVFXSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto* const res = ctx.ww.TryGetResource<::Engine::VFX::VFXCatalogResource>();

		auto& ids = ctx.ww.CreateResource<VFXIds>();

		if (!res)
		{
			Log::error(Log::kInitAllGameVFXSystem, "Failed to get a pointer to VFXCatalogResource (may not be registered in worldResource).");
			return;
		}

		GameVFXCatalogBuilder builder{};
		RegisterVFXDTO(builder);

		auto& cat = res->catalog;

		builder.Build(cat, ids);
		if (!builder.Ok())
		{
			Log::info(Log::kGameVFXCatalogBuilder, "There is an error in the build result of AudioCatalogBuilder");
		}


	}
}