#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include <cstdint>


namespace Engine::Time
{
	struct WorldClockData
	{
		// 旧バージョン
		float now = 0.0f; // 単調増加の世界時刻(秒) max: 3.402823466 E + 38
		float dt = 0.0f; // 直近フレームのΔt()
		float scale = 1.0f; // スローや倍速用
		bool paused = false;// ポーズ用
		// 

		float frameDt = 0.f;// 実測Δt
		float timeScale = 1.f;// スローモ

		// 固定
		float fixedDt = 1.f / 60; // 固定Δt（推奨 60Hz）
		float accumulator = 0.f; // フレーム間の持ち越し
		float alpha = 0.f;// 補間率 accumulator/fixedDt
		int stepsThisFrame = 0;	//今フレで回した固定ステップ回数
		uint64_t frame = 0;// 描画フレーム数	uint64_t = 1.8 * 10^19 | 260fps * 60sec * 60min * 24h * 365days * 100year = 7.56 * 10^11
		uint64_t tick = 0;// 固定ステップの総回数
	};

	class WorldClockSystem
	{
	public:
		static void InitWorldClock(Engine::ECS::EntityMgr& ecs);
		static void TickWorldClock(Engine::ECS::EntityMgr& ecs, float deltaTime);

		static void InitWorldClock(Engine::WorldSystem::Core::WorldCtx& ctx);

		// フレーム先頭でdtの取り込みと「今フレで回せる回数を計算」
		static void BeginFrame(Engine::WorldSystem::Core::WorldCtx& ctx, float realDt);

		// 固定ステップが残っていれば1回分を消費
		static bool PopFixedStep(Engine::WorldSystem::Core::WorldCtx& ctx);

		// フレーム末尾: 描画補間率とフレームカウント更新
		static void EndFrame(Engine::WorldSystem::Core::WorldCtx& ctx);
	};

	// read only
	[[nodiscard]] inline const WorldClockData& worldClock(const Engine::ECS::EntityMgr& ecs) noexcept
	{
		return ecs.getResource<WorldClockData>();
	}

	// writable
	[[nodiscard]] inline WorldClockData& worldClock(Engine::ECS::EntityMgr& ecs) noexcept
	{
		return ecs.getResource<WorldClockData>();
	}

	//[[nodiscard]] inline WorldClockData& worldClock(Engine::World::Core::WorldCtx& ctx) noexcept
	//{
	//	return 
	//}
}