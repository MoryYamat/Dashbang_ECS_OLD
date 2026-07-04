#include "Engine/VFX/Public/VFXAPI.hpp"
#include "Engine/VFX/Internal/VFXTypes.hpp"

#include "Engine/VFX/Internal/VFXLog.hpp"

#include "Engine/IO/Public/FileSystemAPI.hpp"

#include <memory>

namespace Engine::VFX
{
	struct VFXCatalog::Impl
	{
		VFXAssetDB db;
		VFXNameTable names;

		struct ResolvedEntry
		{
			std::filesystem::path absPath;
			bool resolved = false;
		};

		mutable std::vector<ResolvedEntry> resolvedCache;
	};

	VFXCatalog::VFXCatalog()
		: impl_(std::make_unique<Impl>())
	{

	}


	VFXCatalog::~VFXCatalog()
	{
		// impl_.reset();
	}

	// move
	VFXCatalog::VFXCatalog(VFXCatalog&& other) noexcept
		: impl_(std::move(other.impl_))
	{

	}

	VFXCatalog& VFXCatalog::operator=(VFXCatalog&& other)noexcept
	{
		if (this != &other)
		{
			impl_.reset();
			impl_ = std::move(other.impl_);
			// other.impl_.reset();
		}

		return *this;
	}

	VFXID VFXCatalog::register_vfx(std::string name, VFXDef def)
	{
		// ä˘Ç…ë∂ç›Ç∑ÇÈñºëOÇ»ÇÁé∏îs(çƒìoò^ã÷é~)
		if (impl_->names.find(std::string_view{ name }).is_valid())
		{
			// error log
			const VFXID existing = impl_->names.find(std::string_view{ name });
			std::string msg = "Duplicate VFX name: '" + name + "' existing_id=" + std::to_string(existing.value());
			Log::warn(Log::kCatalog, msg.c_str());


			return VFXID::Invalid();
		}

		const VFXID id = impl_->db.register_vfx(std::move(def));
		const bool ok = impl_->names.register_name(std::move(name), id);
		if (!ok)
		{
			Log::error(Log::kCatalog, "register_name failed (unexpected)");
			return VFXID::Invalid();
		}

		return id;
	}

	const VFXDef* VFXCatalog::try_get(VFXID id) const noexcept
	{
		return impl_->db.try_get(id);
	}

	VFXID VFXCatalog::find(std::string_view name) const noexcept
	{
		return impl_->names.find(name);
	}

	std::optional<std::filesystem::path>
		VFXCatalog::get_or_resolve_abs_path(VFXID id, const Engine::IO::IPathResolver& resolver) const
	{
		if (!id.is_valid())
			return std::nullopt;

		const std::size_t idx = static_cast<std::size_t>(id.value());
		if (idx >= impl_->db.size())
			return std::nullopt;

		const VFXDef* def = try_get(id);
		if (!def)
			return std::nullopt;

		if (impl_->resolvedCache.size() != impl_->db.size())
			impl_->resolvedCache.resize(impl_->db.size());

		auto& slot = impl_->resolvedCache[idx];
		if (slot.resolved)
			return slot.absPath;

		auto absOpt = resolver.TryResolve(def->path); // ç°ÇÕ path=textureVPath Ç∆ÇµÇƒàµÇ§
		if (!absOpt)
			return std::nullopt;

		slot.absPath = std::move(*absOpt);
		slot.resolved = true;
		return slot.absPath;
	}
}