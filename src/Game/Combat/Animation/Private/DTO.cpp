#include "Game/Combat/Animation/Internal/DTO.hpp"

#include "Game/Combat/Animation/Public/AnimationApi.hpp"

#include "Game/Combat/Animation/Private/AnimBuilder.hpp"

namespace
{
	using namespace ::Game::Combat::Animation;

	void make_AnimationDTO(AnimBuilder& builder)
	{
		AnimDTO slash_outward_default;
		slash_outward_default.name = "slash_outward_default";
		slash_outward_default.clip = "slash_outward_trim_default";

		builder.Add(slash_outward_default);
	}
}

namespace Game::Combat::Animation
{
	void RegisterAnimationDTO(AnimBuilder& builder)
	{
		make_AnimationDTO(builder);
	}
}