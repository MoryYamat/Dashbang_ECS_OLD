#include "Engine/VFX/Public/VFXAPI.hpp"

#include "Engine/VFX/Internal/VFXTypes.hpp"

#include "Engine/VFX/Internal/VFXLog.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/IO/Public/FileSystemAPI.hpp"
#include <cstddef>

namespace Engine::VFX
{
	void VFXSystem::Update(const VFXCatalogResource& cat,
		VFXCmdBufferResource& res_cmd,
		VFXActiveInstanceResource& active,
		::Engine::IO::IPathResolver& resolver,
		float dt)
	{
		(void)resolver;

		const auto& catalog = cat.catalog;

		auto& cmds = res_cmd.cmd;


		// cmd Á‰»
		if (!cmds.empty())
		{
			for (const auto& c : cmds.cmds)
			{
				if (!c.id.is_valid())
				{
					Log::error(Log::kSystem, "invalid id");
					continue;
				}

				const auto def = catalog.try_get(c.id);
				if (!def)
				{
					Log::warn(Log::kSystem, "VFXDef not found for id");
					continue;
				}

				float ttl = (c.ttlOverride >= 0.0f) ? c.ttlOverride : def->duration;
				if (ttl <= 0.0f)
				{
					Log::warn(Log::kSystem, "Invalid ttl (<=0). skipped");
					continue;
				}


				VFXActiveInstanceResource::VFXInstance inst{};
				inst.id = c.id;
				inst.trs = c.trs;
				inst.t = 0.0f;
				inst.ttl = ttl;

				active.instances.emplace_back(std::move(inst));
			}
			cmds.clear();
		}


		// ¶‘¶Šm”F
		auto& a = active.instances;
		for (std::size_t i = 0; i < a.size();)
		{
			a[i].t += dt;
			if (a[i].t >= a[i].ttl)
			{
				a[i] = a.back();
				a.pop_back();
				continue;
			}

			i++;
		}
	}


	void UpdateAllVFXSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& cat = ctx.ww.GetResource<VFXCatalogResource>();
		auto& cmds = ctx.ww.GetResource<VFXCmdBufferResource>();
		auto& active = ctx.ww.GetResource<VFXActiveInstanceResource>();
		const auto& dt = ctx.ww.GetResource<::Engine::Time::WorldClockData>().frameDt;

		auto& mount = ctx.ww.GetResource<::Engine::IO::MountTable>();
		::Engine::IO::VfsResolver resolver(mount);

		VFXSystem system;
		system.Update(cat, cmds, active, resolver, dt);
	}
}