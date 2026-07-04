#pragma once

#include "Engine/VFX/Public/VFXAPI.hpp"
#include "Game/VFX/Generated/VFXKeys.hpp"
#include <string>

namespace Game::VFX
{
	struct VFXDTO
	{
		VFXKey key = VFXKey::sprite_test;			// stub
		std::string name;
		::Engine::VFX::VFXDef def;
	};
}