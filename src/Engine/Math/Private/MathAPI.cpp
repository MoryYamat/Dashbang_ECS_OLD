#include "Engine/Math/Public/MathAPI.hpp"

#include "Engine/Math/Private/GlmBridge.hpp"

#include <glm/gtc/quaternion.hpp>


namespace Engine::Math
{
	void NormalizeInPlace(Quatf& q) noexcept
	{
		glm::quat g = detail::glm_bridge::ToGlm(q);
		g = glm::normalize(g);
		q = detail::glm_bridge::FromGlm(g);
	}
}