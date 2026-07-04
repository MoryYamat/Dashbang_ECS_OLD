#include "AnimBuilder.hpp"

#include "Game/Combat/Animation/Public/AnimationFwd.hpp"

#include <cstdint>
#include <cstddef>

namespace
{

}

namespace Game::Combat::Animation
{
	AnimBuildResult AnimBuilder::build()
	{
		AnimBuildResult result;

		result.catalog.anims.resize(defs_.size());

		for (std::size_t i = 0; i < defs_.size(); ++i)
		{
			const auto& dto = defs_[i];

			AnimID id{};
			id.v = static_cast<std::uint16_t>(i);

			CanonicalAnimData& dst = result.catalog.anims[i];
			if (!buildOne(dto, id, dst, result.errs))
			{
				// 
			}

			// name->idマップ
			auto [it, inserted] =
				result.catalog.nameToId.emplace(dto.name, id);
			if (!inserted)
			{
				result.errs.err("Anim name '" + dto.name + "' is duplicated");
			}
		}

		return result;
	}

	bool AnimBuilder::buildOne
	(
		const AnimDTO& dto,
		AnimID id,
		CanonicalAnimData& out,
		AnimBuildErrors& err
	)
	{
		bool ok = true;

		out.id = id;
		out.clip = dto.clip;

		return ok;
	}
}