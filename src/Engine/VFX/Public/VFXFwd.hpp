#pragma once

#include "Engine/Core/Public/StrongID.hpp"
#include <cstdint>

namespace Engine::VFX
{
	struct VFXIDTag{};

	using VFXID = ::Engine::Core::StrongID<VFXIDTag, std::uint32_t>;

	class VFXCatalog;
}