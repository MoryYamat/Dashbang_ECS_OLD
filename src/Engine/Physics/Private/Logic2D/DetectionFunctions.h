// 衝突検出の数学的計算関数群

#pragma once

#include <GLM/glm.hpp>

#include "Engine/ECS/Component/Logic2D/ColliderType.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"


namespace Engine::Physics::Logic2D::Collision
{
	// 2D円同士の重なり検出
	bool intersects_Circle2D_Circle2D(const glm::vec2& c1, float r1, const glm::vec2& c2, float r2);

	// 2D円と2D扇形の重なり検出
	bool Intersects_Circle2D_Sector2D(const glm::vec2& circleCenter, float circleRadius,
		const glm::vec2& sectorCenter,
		const glm::vec2& sectorDirection, // normalized
		float sectorAngle,                // in radians
		float sectorRadius);

	// 2D円と線分の交差判定
	bool Intersects_Circle2D_Segment2D(const glm::vec2& circleCenter, float radius, const glm::vec2& segA, const glm::vec2& segB);

	// 2D円と円弧の交差判定
	bool Intersects_Circle2D_Arc2D(
		const glm::vec2& circleCenter,
		float circleRadius,
		const glm::vec2& arcCenter,
		float arcRadius,
		const glm::vec2& arcDirection, // normalized
		float arcAngle                 // radians	
	);

	// 2D円と2D矩形(角度付き)
	bool Intersects_Circle2D_Obb2D(
		const glm::vec2& circleCenter,
		float circleRadius,
		const glm::vec2& obbCenter,
		const glm::vec2& obbHalfExtents,
		const glm::vec2& obbRight, // normalized
		const glm::vec2& obbUp // normalized
	);

	// 2D円と2D矩形(角度なし)
	bool Intersects_Circle2D_Box2D(
		const glm::vec2& circleCenter,
		float circleRadius,
		const glm::vec2& boxCenter,
		const glm::vec2& boxHalfExtents
	);


	// 2D扇形と2D扇形
	bool Intersects_Sector2D_Sector2D(
		const glm::vec2& cA, const glm::vec2& dirA, float angA, float rA,
		const glm::vec2& cB, const glm::vec2& dirB, float angB, float rB
	);

	// 2D扇形と2D矩形(軸付き)
	bool Intersects_Sector2D_Obb2D(
		const glm::vec2& sectorCenter,
		const glm::vec2& sectorDirection,// normalized
		float sectorAngle,
		float sectorRadius,

		const glm::vec2& obbCenter,
		const glm::vec2& obbHalfExtents,
		const glm::vec2& obbRight,// normalized
		const glm::vec2& obbUp// normalized
	);

	// 線分と線分
	bool Intersects_Segment2D_Segment2D(
		const glm::vec2& p1, const glm::vec2& p2,
		const glm::vec2& q1, const glm::vec2& q2
	);

	bool Intersects_Obb2D_Obb2D(
		const glm::vec2& centerA,
		const glm::vec2& halfExtentsA,
		const glm::vec2& axisAX,// normalized
		const glm::vec2& axisAZ,// normalized

		const glm::vec2& centerB,
		const glm::vec2& halfExtentsB,
		const glm::vec2& axisBX,// normalized
		const glm::vec2& axisBZ// normalized
	);

	inline float Cross(const glm::vec2& a, const glm::vec2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	inline bool IsCounterClockwise(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
	{
		return Cross(b - a, c - a) > 0.0f;
	}

	inline bool IsCounterClockwise_IncludeColinear(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
	{
		return Cross(b - a, c - a) >= 0.0f;
	}

	bool pointInCircle(const glm::vec2& p, const glm::vec2& c, float r);

	bool pointInBox(const glm::vec2& p, const glm::vec2& center, const glm::vec2& halfExtents);

	// 
	bool intersectBox2D(const Engine::ECS::Component::Logic2D::Box2D& a, const Engine::ECS::Component::Logic2D::Box2D& b);


	// A pure function that performs collision detection between a circle and an AABB.
	// 円と AABBが交差しているかどうかを判定する
	bool CheckCircleAABBIntersection(
		const glm::vec2& circleCenter,
		float radius,
		const glm::vec2& boxMin,
		const glm::vec2& boxMax
	);

	bool intersectOBB2D_AABB2D(const Engine::ECS::Component::Logic2D::Obb2D& obb, const glm::vec2& aabbMin, const glm::vec2& aabbMax);


	// 補助関数
	inline float DistanceSquared(const glm::vec2& a, const glm::vec2& b)
	{
		glm::vec2 d = a - b;
		return glm::dot(d, d);
	}
}

