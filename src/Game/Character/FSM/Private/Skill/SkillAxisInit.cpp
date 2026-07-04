#include "Game/Character/FSM/Public/SkillAxisApi.hpp"
#include "Game/Character/FSM/Public/SkillTypes.hpp"

#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/InitAxisComponent.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <string_view>
#include <cstdint>

namespace Game::Character::FSM::Skill
{
	using namespace Engine::FSM::Core;
	using namespace Engine::WorldSystem::Core;

	bool InitSkillAxis(Engine::WorldSystem::Core::WorldCtx& ctx, SkillAxisComp& comp)
	{
		return InitAxisComponent<SkillTag>(ctx, comp);
	}
	
	bool InitSkillAxis(Engine::WorldSystem::Core::WorldCtx& ctx, SkillAxisComp& comp, std::string_view fsmName)
	{
		return InitAxisComponent<SkillTag>(ctx, comp, fsmName);
	}

	static bool InitSkillCondTable(Engine::WorldSystem::Core::WorldCtx& ctx, SkillCondTable& out)
	{
		auto* cat = ctx.ww.TryGetResource<FSMCatalog>();
		if (!cat) return false;

		const CanonicalAxis* skillAxis = nullptr;
		for (auto& ax : cat->axes)
		{
			if (ax.axisName == "Skill")
			{
				skillAxis = &ax;
				break;
			}
		}
		if (!skillAxis)
		{
			Engine::Log::Write(Engine::Log::Level::Error, "[SkillAxisInit]", "No CnonicalAxis definition was found for Skill");
			return false;
		}

		const auto& condMap = skillAxis->condIdxByName;

		auto findBit = [&](const char* name, std::uint32_t& dst) -> bool
			{
				auto it = condMap.find(name);
				if (it == condMap.end()) return false;
				dst = it->second;
				return true;
			};

		bool ok = true;

		ok &= findBit("TimeElapsed", out.TimeElapsed.v);
		ok &= findBit("Triggered", out.Triggered.v);

		if (!ok)
		{
			Engine::Log::Write(Engine::Log::Level::Trace, "InitSkillCondTable"
				, "findBit = false → Cond string may not match DTO definition");
		}

		return ok;

	}

	static bool InitSkillStateTable(Engine::WorldSystem::Core::WorldCtx& ctx, SkillStateTable& out)
	{
		auto* cat = ctx.ww.TryGetResource<FSMCatalog>();
		if (!cat) return false;

		const CanonicalAxis* skillAxis = nullptr;
		for (auto& ax : cat->axes)
		{
			if (ax.axisName == "Skill")
			{
				skillAxis = &ax;
				break;
			}
		}
		if (!skillAxis)
		{
			Engine::Log::Write(Engine::Log::Level::Error, "[SkillAxisInit]", "No CnonicalAxis definition was found for Skill");
			return false;
		}

		const auto& stateMap = skillAxis->stateIdxByName;

		auto findBit = [&](const char* name, std::uint32_t& dst) -> bool
			{
				auto it = stateMap.find(name);
				if (it == stateMap.end()) return false;
				dst = it->second;
				return true;
			};

		bool ok = true;

		ok &= findBit("None", out.None.v);
		ok &= findBit("Casting", out.Casting.v);
		ok &= findBit("Active", out.Active.v);
		ok &= findBit("Recovery", out.Recovery.v);
		ok &= findBit("Completed", out.Completed.v);

		if (!ok)
		{
			Engine::Log::Write(Engine::Log::Level::Trace, "InitSkillStateTable"
				, "findBit = false → State string may not match DTO definition");
		}

		return ok;
	}


	static bool InitSkillProfileTable(Engine::WorldSystem::Core::WorldCtx& ctx, SkillProfileTable& out)
	{
		auto* cat = ctx.ww.TryGetResource<FSMCatalog>();
		if (!cat) return false;

		const CanonicalAxis* skillAxis = nullptr;
		for (auto& ax : cat->axes)
		{
			if (ax.axisName == "Skill")
			{
				skillAxis = &ax;
				break;
			}
		}
		if (!skillAxis)
		{
			Engine::Log::Write(Engine::Log::Level::Error, "[SkillAxisInit]", "No CnonicalAxis definition was found for Skill");
			return false;
		}

		const auto& profMap = skillAxis->profileIdxByName;

		auto findBit = [&](const char* name, std::uint32_t& dst) -> bool
			{
				auto it = profMap.find(name);
				if (it == profMap.end()) return false;
				dst = it->second;
				return true;
			};

		bool ok = true;

		ok &= findBit("Default", out.Default.v);

		if (!ok)
		{
			Engine::Log::Write(Engine::Log::Level::Trace, "InitSkillProfileTable"
				, "findBit = false → Profile string may not match DTO definition");
		}

		return ok;
	}

	bool InitSkillFSMTable(Engine::WorldSystem::Core::WorldCtx& ctx,
		SkillCondTable& ctbl, SkillStateTable& stbl, SkillProfileTable& ptbl)
	{
		bool ok = InitSkillCondTable(ctx, ctbl);
		ok &= InitSkillStateTable(ctx, stbl);
		ok &= InitSkillProfileTable(ctx, ptbl);

		return ok;
	}
}