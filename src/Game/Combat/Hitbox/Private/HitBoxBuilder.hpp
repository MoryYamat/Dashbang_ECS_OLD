#pragma once

#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"
#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"
#include "Game/Combat/HitBox/Internal/DTO.hpp"

#include <string>
#include <vector>

namespace Game::Combat::HitBox
{
	struct HitBoxBuilderErrors
	{
		std::vector<std::string> msgs;
		bool ok() const { return msgs.empty(); }
		void err(std::string m) { msgs.emplace_back(std::move(m)); }
	};

	struct HitBoxBuildResult
	{
		HitBoxCatalog catalog;
		HitBoxBuilderErrors errs;
	};

	class HitBoxBuilder
	{
	private:
		std::vector<HitBoxDTO> defs_;

		bool buildOne(
			const HitBoxDTO& dto,
			HitBoxID id,
			CanonicalHitBoxData& out,
			HitBoxBuilderErrors& err
		);

	public:

		HitBoxBuilder() = default;

		void add(HitBoxDTO dto)
		{
			defs_.push_back(std::move(dto));
		}

		HitBoxBuildResult build();
	};
}