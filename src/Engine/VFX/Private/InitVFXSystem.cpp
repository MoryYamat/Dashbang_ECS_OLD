#include "Engine/VFX/Public/VFXAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


#include <iostream>

namespace Engine::VFX
{
	void RegisterVFXResources(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& catalog = ctx.ww.CreateResource<VFXCatalogResource>();
		// cmd buffers 
		auto& buf = ctx.ww.CreateResource<VFXCmdBufferResource>();
		buf.cmd.clear();

		auto& inst = ctx.ww.CreateResource<VFXActiveInstanceResource>();
		inst.instances.clear();
	}

	void InitAllVFXResourceSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		RegisterVFXResources(ctx);
	}
}
