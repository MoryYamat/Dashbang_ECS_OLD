#pragma once

#include "Engine/VFX/Public/VFXFwd.hpp"
#include "Engine/VFX/Public/VFXAPI.hpp"

#include "Engine/Math/Public/MathAPI.hpp"


#include <string_view>
#include <string>
#include <vector>
#include <cstdint>

namespace Engine::VFX
{
	class VFXAssetDB
	{
	private:
		std::vector<VFXDef> vfxes_;


	public:
		VFXID register_vfx(VFXDef def);

		const VFXDef* try_get(VFXID id) const noexcept;

		std::size_t size() const noexcept { return vfxes_.size(); }
	};

	// ìßñæî‰äróp ==========================
	struct TransparentStringEqual
	{
		using is_transparent = void;

		bool operator()(std::string_view a, std::string_view b) const noexcept
		{
			return a == b;
		}

		bool operator()(const std::string& a, const std::string& b) const noexcept
		{
			return a == b;
		}

		bool operator()(const std::string& a, std::string_view b) const noexcept
		{
			return a == b;
		}

		bool operator()(std::string_view a, const std::string& b) const noexcept
		{
			return a == b;
		}
	};

	struct TransparentStringHash
	{
		using is_transparent = void;

		std::size_t operator()(std::string_view sv) const noexcept
		{
			return std::hash<std::string_view>{}(sv);
		}

		std::size_t operator()(const std::string& s) const noexcept
		{
			return std::hash<std::string_view>{}(s);
		}
	};
	// ==========================

	class VFXNameTable
	{
	public:
		// ìoò^ (ìØñºÇ™Ç†Ç¡ÇΩÇÁ false)
		bool register_name(std::string name, VFXID id);

		// åüçı
		VFXID find(std::string_view name) const;

	private:
		std::unordered_map<std::string, VFXID, TransparentStringHash, TransparentStringEqual> name_to_id_;
	};


}