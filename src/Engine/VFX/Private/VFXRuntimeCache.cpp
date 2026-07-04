#include "Engine/VFX/Public/VFXAPI.hpp"

#include "Engine/VFX/Internal/VFXLog.hpp"

#include "Engine/IO/Public/FileSystemAPI.hpp"
#include "Engine/Graphics/Public/GraphicsApi.hpp"

#include <cstddef>
#include <filesystem>

namespace
{
	using namespace Engine::VFX;
	VFXBlend ParseBlend(std::string_view s)
	{
		if (s == "Add" || s == "add")
		{
			return VFXBlend::Add;
		}
		return VFXBlend::Alpha;
	}

	// json parser “±“üŒã
	VFXEntrySprite ParseVFXEntrySprite(const std::filesystem::path& absPath)
	{
		VFXEntrySprite e{};

		// auto j = LoadJson(absPath);

		// e.textureVPath = j.at()
	}
}

namespace Engine::VFX
{
	const VFXRuntimeData* VFXRuntimeCache::get_or_load(
		VFXID id,
		const VFXCatalog& catalog,
		const ::Engine::IO::IPathResolver& resolver,
		::Engine::Graphics::ITextureProvider& texProvider
	)
	{
		if (!id.is_valid())
		{
			Log::error(Log::kCache, "Invalid id (unexpected)");
			return nullptr;
		}

		const std::size_t idx = static_cast<std::size_t>(id.value());
		if (cache_.size() <= idx) cache_.resize(idx + 1);

		if (cache_[idx].has_value())
			return &*cache_[idx];

		const VFXDef* def = catalog.try_get(id);
		if (!def) return nullptr;

		auto texAbs = resolver.TryResolve(def->path);
		if (!texAbs) return nullptr;

		auto tex = texProvider.get_or_load_texture(*texAbs);
		if (!tex.is_valid())return nullptr;

		VFXRuntimeData rt{};
		rt.kind = def->kind;
		rt.blend = def->blend;
		rt.defaultScale = def->defaultScale;
		rt.tex = tex;
		rt.duration = def->duration;

		cache_[idx] = std::move(rt);
		return &*cache_[idx];
	}
}