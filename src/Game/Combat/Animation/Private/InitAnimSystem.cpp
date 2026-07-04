#include "Game/Combat/Animation/Public/AnimationAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Log/Public/LogAPI.hpp"

#include "Game/Combat/Animation/Internal/AnimTypes.hpp"
#include "Game/Combat/Animation/Private/AnimBuilder.hpp"

namespace Game::Combat::Animation
{
	void InitAllAnimSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		AnimBuilder builder;
		RegisterAnimationDTO(builder);

		// build
		AnimBuildResult result;
		result = builder.build();

		// エラー処理
		if (!result.errs.ok())
		{
			for (const auto& msg : result.errs.msgs)
			{
				::Engine::Log::Write(::Engine::Log::Level::Error, "AnimBuildResult", msg);
			}
		}

		// リソース追加
		auto& catalog = ctx.ww.CreateResource<AnimCatalog>();
		catalog = result.catalog;

		// buffer 作成
		auto buffer = ctx.ww.CreateResource<PlayAnimRequestBuffer>();
		buffer.clear();
	}
}