#pragma once

#include "Game/Combat/Animation/Internal/AnimTypes.hpp"
#include "Game/Combat/Animation/Internal/DTO.hpp"

#include <string>
#include <vector>

namespace Game::Combat::Animation
{
	struct AnimBuildErrors
	{
		std::vector<std::string> msgs;
		bool ok() const { return msgs.empty(); }
		void err(std::string m) { msgs.emplace_back(std::move(m)); }
	};

	struct AnimBuildResult
	{
		AnimCatalog catalog;
		AnimBuildErrors errs;
	};

	class AnimBuilder
	{
	private:
		std::vector<AnimDTO> defs_;


		bool buildOne
		(
			const AnimDTO& dto,
			AnimID id,
			CanonicalAnimData& out,
			AnimBuildErrors& err
		);

	public:
		AnimBuilder() = default;

		void Add(AnimDTO& dto)
		{
			defs_.push_back(std::move(dto));
		}

		AnimBuildResult build();
	};
}