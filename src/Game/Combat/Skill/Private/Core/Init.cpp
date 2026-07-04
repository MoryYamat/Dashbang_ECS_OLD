#include "Game/Combat/Skill/Public/CoreApi.hpp"
#include "Game/Combat/Skill/Public/RuntimeAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/Log/Public/LogApi.hpp"

#include "Game/Combat/Skill/Private/Core/SkillCatalogBuilder.hpp"

namespace Game::Combat::Skill
{
	using namespace Engine::WorldSystem::Core;

	static SkillCatalog InitSkillCatalog(SkillCatalogBuilder& builder, const Engine::FSM::Core::FSMCatalog& fsmCat)
	{
		SkillBuildResult result;
		result = builder.build(fsmCat);

		if (!result.err.ok())
		{
			for (const auto& msg : result.err.msgs)
			{
				Engine::Log::Write(Engine::Log::Level::Error, "Init Skill Catalog", msg);
			}
		}

		return result.catalog;
	}

	// (呼出し順注意): FSMEngine 初期化後に呼び出す必要あり
	void InitAllSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// auto& builder = ctx.ww.CreateResource<SkillCatalogBuilder>();
		SkillCatalogBuilder builder{};
		RegisterSkillDef(builder);

		const auto* fsmCat = ctx.rw.TryGetResource<Engine::FSM::Core::FSMCatalog>();
		if (!fsmCat)
		{
			Engine::Log::Write(Engine::Log::Level::Error, "Init All Skill System", "Failed to get FSMCatalog: nullptr.");
			return;
		}
		
		auto& catalog = ctx.ww.CreateResource<SkillCatalog>();
		catalog = InitSkillCatalog(builder, *fsmCat);

		Runtime::InitAllSkillRuntimeSystem(ctx);
	}
}