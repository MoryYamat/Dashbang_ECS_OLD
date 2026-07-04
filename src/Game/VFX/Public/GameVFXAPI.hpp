#pragma once

#include "Engine/VFX/Public/VFXFwd.hpp"
#include "Game/VFX/Generated/VFXKeys.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include <cstddef>
#include <array>

namespace Game::VFX
{
	struct VFXIds
	{
		std::array<::Engine::VFX::VFXID, static_cast<std::size_t>(VFXKey::Count)> ids;

		VFXIds()
		{
			ids.fill(::Engine::VFX::VFXID::Invalid());
		}

		::Engine::VFX::VFXID get(VFXKey key) const noexcept
		{
			return ids[static_cast<std::size_t>(key)];
		}

		void set(::Engine::VFX::VFXID id, VFXKey key) noexcept
		{
			ids[static_cast<std::size_t>(key)] = id;
		}
	};

	void InitAllGameVFXSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
}