#include "Game/Combat/HitBox/Public/HitBoxApi.hpp"
#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"

#include "Game/Combat/HitBox/Private/HitBoxBuilder.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/Log/Public/LogAPI.hpp"

namespace Game::Combat::HitBox
{
	void InitAllHitBoxSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// ---- 初期化 ---- 
		// DTO定義 取り込み
		HitBoxBuilder builder;		
		RegisterHitBoxDTO(builder);

		// build
		HitBoxBuildResult result;
		result = builder.build();

		// エラー処理
		if (!result.errs.ok())
		{
			for (const auto& err : result.errs.msgs)
			{
				Engine::Log::Write(Engine::Log::Level::Error, "HitBoxBuildResult",
					err);
			}
		}

		// 追加
		auto& catalog = ctx.ww.CreateResource<HitBoxCatalog>();
		catalog = result.catalog;

		// buffer 作成 / 初期化
		auto& buf = ctx.ww.CreateResource<HitBoxSpawnRequestBuffer>();
		buf.clear();
	}
}