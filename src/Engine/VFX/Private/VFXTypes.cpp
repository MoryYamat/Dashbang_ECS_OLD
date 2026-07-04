#include "Engine/VFX/Internal/VFXTypes.hpp"
#include "Engine/VFX/Internal/VFXLog.hpp"

#include "Engine/VFX/Public/VFXFwd.hpp"

#include <cstdint>
#include <cstddef>

namespace Engine::VFX
{
	VFXID VFXAssetDB::register_vfx(VFXDef def)
	{
		auto idx = static_cast<std::uint32_t>(vfxes_.size());
		vfxes_.push_back(std::move(def));
		return VFXID{ idx };
	}

	const VFXDef* VFXAssetDB::try_get(VFXID id) const noexcept
	{
		if (!id.is_valid())
		{
			Log::error(Log::kAssetDB, "invalid vfx id");
			return nullptr;
		}

		const auto idx = static_cast<std::size_t>(id.value());
		if(idx >= vfxes_.size())
		{
			Log::fatal(Log::kAssetDB, "invalid index (unexpected)");
			return nullptr;
		}

		return &vfxes_[idx];
	}

	// ===================== Name Table ==========================

	// “o˜^ (“¯–¼‚ª‚ ‚Á‚½‚ç false)
	bool VFXNameTable::register_name(std::string name, VFXID id)
	{
		if (!id.is_valid()) return false;

		auto [it, inserted] = name_to_id_.emplace(std::move(name), id);
		return inserted;
	}

	// ŒŸõ
	VFXID VFXNameTable::find(std::string_view name) const
	{
		auto it = name_to_id_.find(name);
		if (it == name_to_id_.end())
		{
			return VFXID::Invalid();
		}
		return it->second;
	}
}