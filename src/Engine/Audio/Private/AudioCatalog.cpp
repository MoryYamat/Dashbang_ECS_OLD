#include "Engine/Audio/Internal/AudioTypes.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"
#include "Engine/Audio/Public/AudioFwd.hpp"

#include "Engine/Audio/Internal/AudioLog.hpp"
#include "Engine/IO/Public/FileSystemAPI.hpp"

#include <optional>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>


namespace Engine::Audio
{

	// ========================= AUDIO CATALOG ======================
	struct AudioCatalog::Impl
	{
		AudioAssetDB db;
		SoundNameTable names;

		// é¿çséûÉLÉÉÉbÉVÉÖ (AudioCatalogêÍóp)
		struct ResolvedEntry
		{
			std::filesystem::path absPath;
			bool resolved = false;
		};
		mutable std::vector<ResolvedEntry> resolvedCache;
	};

	AudioCatalog::AudioCatalog()
		:impl_(std::make_unique<Impl>())
	{

	}

	AudioCatalog::~AudioCatalog()
	{
		// impl_.reset();
	}

	AudioCatalog::AudioCatalog(AudioCatalog&& other) noexcept
		: impl_(std::move(other.impl_))
	{
		// other.impl_.reset();
	}

	AudioCatalog& AudioCatalog::operator=(AudioCatalog&& other) noexcept
	{
		if (this != &other)
		{
			impl_.reset();
			impl_ = std::move(other.impl_);
			other.impl_.reset();
		}

		return *this;
	}

	SoundID AudioCatalog::register_sound(std::string name, SoundDef def)
	{
		// ä˘Ç…ë∂ç›Ç∑ÇÈñºëOÇ»ÇÁé∏îs(çƒìoò^ã÷é~)
		if (impl_->names.find(std::string_view{ name }).is_valid())
		{
			// error log
			const SoundID existing = impl_->names.find(std::string_view{ name });
			std::string msg = "Duplicate sound name: '" + name + "' existing_id=" + std::to_string(existing.value());
			Log::warn(Log::kCatalog, msg.c_str());


			return SoundID::Invalid();
		}

		SoundID id = impl_->db.register_sound(std::move(def));
		const bool ok = impl_->names.register_name(std::move(name), id);
		if (!ok)
		{
			Log::error(Log::kCatalog, "register_name failed (unexpected)");

			return SoundID::Invalid();
		}

		return id;
	}

	SoundID AudioCatalog::find(std::string_view name) const noexcept
	{
		return impl_->names.find(name);
	}

	const SoundDef* AudioCatalog::try_get(SoundID id) const noexcept
	{
		return impl_->db.try_get(id);
	}


	std::optional<std::filesystem::path>
		AudioCatalog::get_or_resolve_abs_path(SoundID id, const Engine::IO::IPathResolver& resolver) const
	{
		if (!id.is_valid())
			return std::nullopt;

		const std::size_t idx = static_cast<std::size_t>(id.value());
		if (idx >= impl_->db.size())
			return std::nullopt;

		const SoundDef* def = try_get(id);
		if (!def) 
			return std::nullopt;

		if (impl_->resolvedCache.size() != impl_->db.size())
			impl_->resolvedCache.resize(impl_->db.size());

		auto& slot = impl_->resolvedCache[idx];
		if (slot.resolved)
			return slot.absPath;

		auto absOpt = resolver.TryResolve(def->path);
		if (!absOpt) return std::nullopt;

		slot.absPath = std::move(*absOpt);
		slot.resolved = true;
		return slot.absPath;
	}
}



// é¿çsånÇ≈ÇÕÅAidÇ≈âåàÇ∑ÇÈ
// 
// struct CombatSounds
// {
// 	SoundID hit_light;
// 	SoundID hit_heavy;
// 	SoundID swing;
// };
// 
// CombatSounds sounds;
// sounds.hit_light = catalog.find("HitLight");
// sounds.hit_heavy = catalog.find("HitHeavy");
// sounds.swing = catalog.find("Swing");
// 
// cmd.PlaySound(sounds.hit_light, pos, volumeScale);
