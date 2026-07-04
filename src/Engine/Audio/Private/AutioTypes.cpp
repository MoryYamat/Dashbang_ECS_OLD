#include "Engine/Audio/Internal/AudioTypes.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"

#include "Engine/Audio/Internal/AudioLog.hpp"

#include <cstdint>
#include <cstddef>
#include <string>


namespace Engine::Audio
{
	// ====================== AudioAssetDB =======================
	SoundID AudioAssetDB::register_sound(SoundDef def)
	{
		const std::uint32_t index = static_cast<std::uint32_t>(sounds_.size());
		sounds_.push_back(std::move(def));
		return SoundID{ index };
	}

	const SoundDef* AudioAssetDB::try_get(SoundID id)const noexcept
	{
		if (!id.is_valid())
		{


			return nullptr;
		}

		const auto index = static_cast<std::size_t>(id.value());
		if (index >= sounds_.size())
		{
			return nullptr;
		}

		return &sounds_[index];
	}


	// ====================== SoundNameTable =========================
	bool SoundNameTable::register_name(std::string name, SoundID id)
	{
		if (!id.is_valid()) return false;

		auto [it, inserted] = name_to_id_.emplace(std::move(name), id);
		return inserted;
	}

	SoundID SoundNameTable::find(std::string_view name) const
	{
		auto it = name_to_id_.find(name);
		if (it == name_to_id_.end())
		{
			return SoundID::Invalid();
		}
		return it->second;
	}
}