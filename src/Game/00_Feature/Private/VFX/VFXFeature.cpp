#include "Game/00_Feature/Public/EffectFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Game/VFX/Public/GameVFXAPI.hpp"

#include "Engine/VFX/Public/VFXAPI.hpp"

namespace Game::Feature::VFX
{
	void VFXFeature::InitGameVFXSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::VFX::InitAllGameVFXSystem(ctx);
	}

	void VFXFeature::Update(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		::Engine::VFX::UpdateAllVFXSystem(ctx);
	}
}