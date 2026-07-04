#pragma once

#include "Engine/FSM/Public/Core/Registry.hpp"
#include "Engine/Public/EngineFwd.hpp"


#include <span>
#include <string>
#include <functional>
#include <string_view>

namespace Engine::FSM::Core
{
	//using RegisterFn = void(*)(FSMRegistry& reg);  // DTO を reg.add(...) する
	using RegisterFn = std::function<void(FSMRegistry& reg)>;  // DTO を reg.add(...) する
	// フィールド名→インデックス解決用の関数型(ゲーム側が提供)
	using FieldResolver = std::function<bool(std::string_view name, std::uint16_t& outIndex)>;

	using FieldResolverProvider = std::function<bool(std::string_view axisName, FieldResolver& out)>;

	void InitFSMEngine(Engine::WorldSystem::Core::WorldCtx& ctx);
	void RegisterAxes(Engine::WorldSystem::Core::WorldCtx& ctx, RegisterFn fn);   // 2)

	bool BuildFSMCatalog(Engine::WorldSystem::Core::WorldCtx& ctx, FieldResolverProvider resolverProvider, BuildStrictness policy = BuildStrictness::Lenient);

	// 便利用：全部やる
	bool InitAllFSMs(Engine::WorldSystem::Core::WorldCtx& ctx,
		RegisterFn registerFn,
		FieldResolverProvider resolverProvider,
		BuildStrictness policy = BuildStrictness::Strict);


	Decision DecideNext_BySingleSlot
	(
		const CanonicalFSM& f,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::uint32_t slotLocal,
		const EnvSnapshot& env
	);

	Decision DecideNext_Slots
	(
		const CanonicalFSM& f,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::span<const std::uint32_t> slots,
		const EnvSnapshot& env
	);

	// 焼きこみ
	void BakeEnvAssemblerPlan
	(
		const CanonicalAxis& ax,
		AxisRuntime& rt,
		FieldResolver  resolveField,
		BuildErrors& err
	);
}

namespace Engine::FSM::Debug
{

	void PrintFSMInfo(Engine::ECS::Core::Entity e, const Engine::FSM::Core::AxisInstance& inst, Engine::FSM::Core::Decision& d);
}