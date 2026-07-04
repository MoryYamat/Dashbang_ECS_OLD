#include "Game/VFX/Internal/DTO.hpp"
#include "Game/VFX/Internal/GameVFXInternalAPI.hpp"

#include "Game/VFX/Generated/VFXKeys.hpp"

#include "Game/VFX/Private/GameVFXCatalogBuilder.hpp"

#include "Engine/VFX/Public/VFXAPI.hpp"

#include <iostream>

namespace
{
	using namespace Game::VFX;
	using namespace Engine::VFX;

	void make_VFXDTO(GameVFXCatalogBuilder& builder)
	{
		VFXDTO test_sprite;
		test_sprite.key = VFXKey::sprite_test;
		test_sprite.name = "test_slash01";
		test_sprite.def = VFXDef{ .path = "res://Textures/VFX/twirl_01.png", .kind = VFXKind::Sprite, .duration = 0.3 };


		builder.Add(test_sprite);
	}
}

namespace Game::VFX
{
	void RegisterVFXDTO(GameVFXCatalogBuilder& builder)
	{
		make_VFXDTO(builder);
	}
}