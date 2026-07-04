#include "Game/Combat/Skill/Private/Core/SkillCatalogBuilder.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"

#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/ID-NameHelper.hpp"


#include <string>
#include <string_view>
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <algorithm>


namespace Game::Combat::Skill
{
	using namespace Engine::FSM::Core;

	static bool parseEffectKind(std::string_view s, EffectKind& out)
	{
		if (s == "SpawnHitbox" || s == "SpawnHitBox")
		{
			out = EffectKind::SpawnHitBox;
			return true;
		}
		if (s == "PlayAnim")
		{
			out = EffectKind::PlayAnim;
			return true;
		}
		if (s == "ModifyMoveSpeed")
		{
			out = EffectKind::ModifyMoveSpeed;
			return true;
		}
		if (s == "PlaySFX")
		{
			out = EffectKind::PlaySFX;
			return true;
		}
		if (s == "PlayVFX")
		{
			out = EffectKind::PlayVFX;
			return true;
		}

		return false;
	}

	SkillBuildResult SkillCatalogBuilder::build(const Engine::FSM::Core::FSMCatalog& fsmCat)
	{
		SkillBuildResult result;

		result.catalog.skills.resize(defs_.size());

		bool ok = true;

		for (std::size_t i = 0; i < defs_.size(); ++i)
		{
			const auto& dto = defs_[i];
			CanonicalSkillData& dst = result.catalog.skills[i];
			if (!buildSkill(dto, SkillID( static_cast<std::uint16_t>(i)) , fsmCat, dst, result.err))
			{
				// 続行 (全　skill のエラーを報告)

				ok = false;
			}
		}

		// name to id mapping を作成
		result.catalog.nameToId.clear();
		result.catalog.nameToId.reserve(result.catalog.skills.size());
		for (const auto& s : result.catalog.skills)
		{
			if (s.name.empty())
			{
				continue;
			}

			auto [it, inserted] = result.catalog.nameToId.emplace(s.name, s.id);
			if (!inserted)
			{
				result.err.err(
					"Duplicate skill name '" + s.name + "' in SkillCatalogBuilder::build()"
				);
				ok = false;
			}
		}

		return result;
	}

	bool SkillCatalogBuilder::buildSkill(const SkillDefDTO& dto,
		SkillID assignedID,
		const Engine::FSM::Core::FSMCatalog& fsmCat,
		CanonicalSkillData& out,
		SkillBuilderErrors& errors)
	{
		bool ok = true;

		out.id = assignedID;
		out.name = dto.name;
		out.phases.clear();
		out.effects.clear();

		// --------------------------------------------------------------------
		// 1. Axis "Skill"を探す
		// --------------------------------------------------------------------
		const CanonicalAxis* axis = nullptr;
		for (const auto& ax : fsmCat.axes)
		{
			if (ax.axisName == "Skill")// 
			{
				axis = &ax;
				break;
			}
		}

		if (!axis)
		{
			errors.err("Skill '" + dto.name + 
				"': FSM Axis 'Skill' not found in FSMCatalog");
			return false;
		}

		out.axis = axis->axis;

		// --------------------------------------------------------------------
		// 2. FSM 名 → FSMID
		// --------------------------------------------------------------------
		std::string fsmName = dto.fsm.empty() ? "Basic" : dto.fsm;

		auto itFSM = axis->fsmIdxByName.find(fsmName);
		if (itFSM == axis->fsmIdxByName.end())
		{
			errors.err("Skill '" + dto.name +
				"': Unkown FSM '" + dto.fsm + "'");
			ok = false;
		}
		else
		{
			out.fsm.v = itFSM->second;
		}

		// --------------------------------------------------------------------
		// 3. profile名→ProfileID
		// --------------------------------------------------------------------
		std::string profName = dto.profile.empty() ? "Default" : dto.profile;

		auto itProf = axis->profileIdxByName.find(profName);
		if (itProf == axis->profileIdxByName.end())
		{
			errors.err("Skill '" + dto.name + 
				"': unknown profile '" + profName + "' for Axis Skill");
			ok = false;
		}
		else
		{
			out.profile.v = itProf->second;
		}
		
		if (!ok) return false;


		// --------------------------------------------------------------------
		// 4. Phase -> StateID
		// --------------------------------------------------------------------
		

		for (const auto& phase : dto.phases)
		{
			auto itState = axis->stateIdxByName.find(phase.name);
			if (itState == axis->stateIdxByName.end())
			{
				errors.err("Skill '" + dto.name +
					"': Phase name '" + phase.name +
					"' does not match any state in Axis '" + dto.fsm + "'");
				ok = false;
				continue;
			}

			CanonicalSkillPhase cph;
			cph.state.v = itState->second;
			cph.duration = phase.duration;

			out.phases.push_back(cph);
		}

		// --------------------------------------------------------------------
		// 5. Effect → StateID 正規化
		// --------------------------------------------------------------------
		for (const auto& eff : dto.effects)
		{
			auto itState = axis->stateIdxByName.find(eff.phase);
			if (itState == axis->stateIdxByName.end())
			{
				errors.err("Skill '" + dto.name +
					"': Effect phase '" + eff.phase +
					"' not found in Axis '" + dto.fsm + "'");
				ok = false;
				continue;
			}

			EffectKind kind;
			if (!parseEffectKind(eff.kind, kind))
			{
				errors.err("Skill '" + dto.name +
					"': Unkown EffectKind '" + eff.kind + "'");
				ok = false;
				continue;
			}

			CanonicalSkillEffect cef;
			cef.state.v = itState->second;
			cef.timeOffset = eff.timeOffset;
			cef.kind = kind;
			cef.lifetime = eff.lifetime;
			cef.value = eff.value;

			out.effects.push_back(cef);
		}

		// 同じStateに対する異なるEffectをTimeoffsetでソート
		std::stable_sort(out.effects.begin(), out.effects.end(),
			[](const CanonicalSkillEffect& a, const CanonicalSkillEffect& b)
			{
				if (a.state.v != b.state.v)
					return a.state.v < b.state.v;
				
				if(a.timeOffset != b.timeOffset)
					return a.timeOffset < b.timeOffset;

				return static_cast<std::uint8_t>(a.kind) < static_cast<std::uint8_t>(b.kind);
			});

		return ok;
	}
}