#pragma once
#include "Game/Combat/Skill/Internal/Core/CoreDTO.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"

#include <string>
#include <vector>

namespace Game::Combat::Skill
{
	struct SkillBuilderErrors
	{
		std::vector<std::string> msgs;
		bool ok() const { return msgs.empty(); }
		void err(std::string m) { msgs.emplace_back(std::move(m)); }
	};

	struct SkillBuildResult
	{
		SkillCatalog catalog;
		SkillBuilderErrors err;
	};

	class SkillCatalogBuilder
	{
	public:
		void add(SkillDefDTO dto)
		{
			defs_.push_back(std::move(dto));
		}

		// FSMCatalog
		SkillBuildResult build(const Engine::FSM::Core::FSMCatalog& fsmCat);

	private:
		std::vector<SkillDefDTO> defs_;

		bool buildSkill(const SkillDefDTO& dto,
			SkillID assignedID,
			const Engine::FSM::Core::FSMCatalog& fsmCat,
			CanonicalSkillData& out,
			SkillBuilderErrors& errors);
	};
}