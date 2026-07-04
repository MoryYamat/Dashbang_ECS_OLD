#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/Math/Public/MathAPI.hpp"


namespace Engine::Math::detail::glm_bridge
{
	// Vec <-> glm::vec
	[[nodiscard]] inline glm::vec2 ToGlm(const Vec2f& v) noexcept
	{
		return glm::vec2(v.x, v.y);
	}

	[[nodiscard]] inline glm::vec3 ToGlm(const Vec3f& v) noexcept
	{
		return glm::vec3(v.x, v.y, v.z);
	}

	[[nodiscard]] inline glm::vec4 ToGlm(const Vec4f& v) noexcept
	{
		return glm::vec4(v.x, v.y, v.z, v.w);
	}

	[[nodiscard]] inline Vec2f FromGlm(const glm::vec2& v) noexcept
	{
		return Vec2f{ v.x, v.y };
	}

	[[nodiscard]] inline Vec3f FromGlm(const glm::vec3& v) noexcept
	{
		return Vec3f{ v.x, v.y, v.z};
	}

	[[nodiscard]] inline Vec4f FromGlm(const glm::vec4& v) noexcept
	{
		return Vec4f{ v.x, v.y, v.z, v.w };
	}

	[[nodiscard]] inline glm::quat ToGlm(const Quatf& q) noexcept
	{
		return glm::quat(q.w, q.x, q.y, q.z);
	}

	[[nodiscard]] inline Quatf FromGlm(const glm::quat& q) noexcept
	{
		return Quatf{ q.x, q.y, q.z, q.w };
	}

	// TRS
	struct GlmTRS
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
	};

	[[nodiscard]] inline GlmTRS ToGlm(const TRS& t) noexcept
	{
		return GlmTRS{
			.translation = ToGlm(t.translation),
			.rotation = ToGlm(t.rotation),
			.scale = ToGlm(t.scale),
		};
	}

	[[nodiscard]] inline TRS FromGlm(const GlmTRS& t) noexcept
	{
		TRS out;
		out.translation = FromGlm(t.translation);
		out.rotation = FromGlm(t.rotation);
		out.scale = FromGlm(t.scale);
		return out;
	}
}