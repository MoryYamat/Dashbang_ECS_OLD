#include "Game/VFX/Private/GameVFXCatalogBuilder.hpp"

#include "Engine/VFX/Public/VFXAPI.hpp"
#include "Game/VFX/Public/GameVFXAPI.hpp"

namespace Game::VFX
{
	void GameVFXCatalogBuilder::Build(::Engine::VFX::VFXCatalog& catalog, VFXIds& ids)
	{
		for (const auto e : defs_)
		{
			const auto id = catalog.register_vfx(e.name, e.def);
			if (!id.is_valid())
			{
				errs_.error("VFX registration failed (invalid vfx id)");
				continue;
			}
			ids.set(id, e.key);
		}
	}
}