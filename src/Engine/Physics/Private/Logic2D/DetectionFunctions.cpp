#include "DetectionFunctions.h"

#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"



#include <iostream>
#include <string>

bool Engine::Physics::Logic2D::Collision::intersects_Circle2D_Circle2D(const glm::vec2& c1, float r1, const glm::vec2& c2, float r2)
{
	float distanceSq = DistanceSquared(c1, c2);
	float raidusSum = r1 + r2;
	return distanceSq <= raidusSum * raidusSum;
}

//glm::vec2 toCircle = circleCenter - sectorCenter;
//float distSq = glm::dot(toCircle, toCircle);
//float maxRange = sectorRadius + circleRadius;

//// まず距離で除外
//if (distSq > maxRange * maxRange)
//	return false;

//// 次に角度で除外（atan2を用いた明示的比較）
//float angleToCircle = std::atan2(toCircle.y, toCircle.x);
//float angleDir = std::atan2(sectorDirection.y, sectorDirection.x);

//float angleDiff = std::abs(angleToCircle - angleDir);
//if (angleDiff > glm::pi<float>())
//	angleDiff = glm::two_pi<float>() - angleDiff;

//if (angleDiff > sectorAngle * 0.5f)
//	return false;

//return true;

bool Engine::Physics::Logic2D::Collision::Intersects_Circle2D_Sector2D(const glm::vec2& circleCenter, float circleRadius,
	const glm::vec2& sectorCenter,
	const glm::vec2& sectorDirection, // normalized
	float sectorAngle,                // in radians
	float sectorRadius)
{
	glm::vec2 toCircle = circleCenter - sectorCenter;
	float distSq = glm::dot(toCircle, toCircle);
	float maxDist = sectorRadius + circleRadius;

	// 距離が遠すぎる
	if (distSq > maxDist * maxDist)
		return false;

	// ここから精密かすり判定用：
	glm::vec2 leftVec = Engine::Math::Logic2D::RotateVec2FromZForward(sectorDirection, +sectorAngle * 0.5f);
	glm::vec2 rightVec = Engine::Math::Logic2D::RotateVec2FromZForward(sectorDirection, -sectorAngle * 0.5f);

	glm::vec2 tipLeft = sectorCenter + leftVec * sectorRadius;
	glm::vec2 tipRight = sectorCenter + rightVec * sectorRadius;

	// 左辺と円交差
	if (Intersects_Circle2D_Segment2D(circleCenter, circleRadius, sectorCenter, tipLeft))
		return true;

	// 右辺と円交差
	if (Intersects_Circle2D_Segment2D(circleCenter, circleRadius, sectorCenter, tipRight))
		return true;

	// normalizeしないでそのままcos²比較
	float dot = glm::dot(toCircle, sectorDirection);
	float dotSq = dot * dot;
	float cosThreshold = std::cos(sectorAngle * 0.5f);
	float cosThresholdSq = cosThreshold * cosThreshold;

	// 投影長が有効な角度内か（余弦定理ベース）
	if (dotSq >= distSq * (cosThresholdSq - 0.0001f))
		return true;

	// 弧の端と円がかすっているか（外縁）
	if (Intersects_Circle2D_Arc2D(circleCenter, circleRadius, sectorCenter, sectorRadius, sectorDirection, sectorAngle))
		return true;


	return false;
}

bool Engine::Physics::Logic2D::Collision::Intersects_Circle2D_Segment2D(const glm::vec2& circleCenter, float radius, const glm::vec2& segA, const glm::vec2& segB)
{
	//
	glm::vec2 ab = segB - segA;
	glm::vec2 ac = circleCenter - segA;
	float t = glm::dot(ac, ab) / glm::dot(ab, ab);
	t = glm::clamp(t, 0.0f, 1.0f);
	glm::vec2 closest = segA + ab * t;
	glm::vec2 diff = circleCenter - closest;
	return glm::dot(diff, diff) <= radius * radius;
}

bool Engine::Physics::Logic2D::Collision::Intersects_Circle2D_Arc2D(
	const glm::vec2& circleCenter,
	float circleRadius,
	const glm::vec2& arcCenter,
	float arcRadius,
	const glm::vec2& arcDirection, // normalized
	float arcAngle                 // radians	
)
{
	glm::vec2 toCircle = circleCenter - arcCenter;
	float distSq = glm::dot(toCircle, toCircle);
	float arcRadiusSq = arcRadius * arcRadius;

	// 中心との距離が外円に近ければ候補(近すぎる／遠すぎるを判定)
	float minDist = arcRadius - circleRadius;
	float maxDist = arcRadius + circleRadius;
	if (distSq < minDist * minDist || distSq > maxDist * maxDist)
		return false;

	// 有効角度内かどうか
	float dot = glm::dot(toCircle, arcDirection);
	float dotSq = dot * dot;
	float cosThreshold = std::cos(arcAngle * 0.5f);
	float cosThresholdSq = cosThreshold * cosThreshold;

	return dotSq >= distSq * (cosThresholdSq - 0.0001f);
}

bool Engine::Physics::Logic2D::Collision::Intersects_Circle2D_Obb2D(
	const glm::vec2& circleCenter,
	float circleRadius,
	const glm::vec2& obbCenter,
	const glm::vec2& obbHalfExtents,
	const glm::vec2& obbRight, // normalized
	const glm::vec2& obbUp // normalized
)
{
	// 円野中心をOBBローカル空間へ変換
	glm::vec2 toCircle = circleCenter - obbCenter;

	// OBBの軸ベクトルで射影 -> ローカル座標系へ
	float localX = glm::dot(toCircle, obbRight);
	float localY = glm::dot(toCircle, obbUp);
	glm::vec2 localCirclePos = { localX, localY };

	// AABB と円との交差判定に委譲（center = (0,0)）
	return Intersects_Circle2D_Box2D(
		localCirclePos,
		circleRadius,
		glm::vec2(0.0f),       // ローカル空間での Box center
		obbHalfExtents
	);
}


bool Engine::Physics::Logic2D::Collision::Intersects_Circle2D_Box2D(
	const glm::vec2& circleCenter,
	float circleRadius,
	const glm::vec2& boxCenter,
	const glm::vec2& boxHalfExtents
)
{
	// 円の中心を Box のローカル空間で相対位置に変換
	glm::vec2 local = circleCenter - boxCenter;

	// AABB 内の最近点を求める（Clamp で収まる点）
	glm::vec2 clamped = glm::clamp(local, -boxHalfExtents, boxHalfExtents);

	// 最近点と円中心の距離を測る
	glm::vec2 diff = local - clamped;
	float distSq = glm::dot(diff, diff);

	return distSq <= circleRadius * circleRadius;

}

bool Engine::Physics::Logic2D::Collision::Intersects_Sector2D_Sector2D(
	const glm::vec2& centerA, const glm::vec2& dirA, float angA, float radiusA,
	const glm::vec2& centerB, const glm::vec2& dirB, float angB, float radiusB
)
{
	// 円の場合の重なり
	float rr = radiusA + radiusB;
	glm::vec2 v = centerB - centerA;
	if (glm::dot(v, v) > rr * rr)
		return false;

	// Aの中心がBに含まれる
	glm::vec2 BA = centerA - centerB;
	float distBASq = glm::dot(BA, BA);
	if (distBASq <= radiusB * radiusB)// 半径
	{
		float cosLim = std::cos(angB * 0.5f);
		if (glm::dot(glm::normalize(BA), dirB) >= cosLim)
			return true;// 角度
	}

	// Bの中心がAに含まれる
	glm::vec2 AB = -BA;
	float distABSq = distBASq;
	if (distABSq <= radiusA * radiusA)
	{
		float cosLim = std::cos(angA * 0.5f);
		if (glm::dot(glm::normalize(AB), dirA) >= cosLim)
			return true;
	}

	return false;
}

bool Engine::Physics::Logic2D::Collision::Intersects_Sector2D_Obb2D(
	const glm::vec2& sectorCenter,
	const glm::vec2& sectorDirection,// normalized
	float sectorAngle,
	float sectorRadius,

	const glm::vec2& obbCenter,
	const glm::vec2& obbHalfExtents,
	const glm::vec2& obbRight,// normalized
	const glm::vec2& obbUp// normalized
)
{
	// 円-距離チェック
	float obbRadiusEstimate = glm::length(obbHalfExtents.x * obbRight + obbHalfExtents.y * obbUp);
	glm::vec2 d = obbCenter - sectorCenter;
	if (glm::dot(d, d) > (sectorRadius + obbRadiusEstimate) * (sectorRadius + obbRadiusEstimate))
		return false;

	// obbの頂点
	glm::vec2 dx = obbRight * obbHalfExtents.x;
	glm::vec2 dy = obbUp * obbHalfExtents.y;

	glm::vec2 corners[4] = {
		obbCenter + dx + dy,
		obbCenter + dx - dy,
		obbCenter - dx - dy,
		obbCenter - dx + dy
	};

	float cosThreshold = std::cos(sectorAngle * 0.5f);

	for (int i = 0; i < 4; ++i)
	{
		glm::vec2 toCorner = corners[i] - sectorCenter;
		float distSq = glm::dot(toCorner, toCorner);
		if (distSq <= sectorRadius * sectorRadius)
		{
			float dot = glm::dot(glm::normalize(toCorner), sectorDirection);
			if (dot >= cosThreshold)
				return true;
		}
	}

	// 
	glm::vec2 left = Engine::Math::Logic2D::RotateVec2FromZForward(sectorDirection, +sectorAngle * 0.5f);
	glm::vec2 right = Engine::Math::Logic2D::RotateVec2FromZForward(sectorDirection, -sectorAngle * 0.5f);
	glm::vec2 tipLeft = sectorCenter + left * sectorRadius;
	glm::vec2 tipRight = sectorCenter + right * sectorRadius;

	glm::vec2 triangle[3] = { sectorCenter, tipLeft, tipRight };

	for (int i = 0; i < 4; ++i)
	{
		glm::vec2 p1 = corners[i];
		glm::vec2 p2 = corners[(i + 1) % 4];

		for (int j = 0; j < 3; ++j)
		{
			glm::vec2 s1 = triangle[j];
			glm::vec2 s2 = triangle[(j + 1) % 3];

			if (Intersects_Segment2D_Segment2D(p1, p2, s1, s2))
				return true;
		}
	}

}

bool Engine::Physics::Logic2D::Collision::Intersects_Segment2D_Segment2D(
	const glm::vec2& p1, const glm::vec2& p2,
	const glm::vec2& q1, const glm::vec2& q2
)
{
	// 線分 (p1, p2) が (q1, q2) をまたいでいるか？（片側ずつ）
	bool ccw1 = IsCounterClockwise(p1, p2, q1);
	bool ccw2 = IsCounterClockwise(p1, p2, q2);

	// 線分 (q1, q2) が (p1, p2) をまたいでいるか？
	bool ccw3 = IsCounterClockwise(q1, q2, p1);
	bool ccw4 = IsCounterClockwise(q1, q2, p2);

	return (ccw1 != ccw2) && (ccw3 != ccw4);

}

bool Engine::Physics::Logic2D::Collision::Intersects_Obb2D_Obb2D(
	const glm::vec2& centerA,
	const glm::vec2& halfExtentsA,
	const glm::vec2& axisAX,// normalized
	const glm::vec2& axisAZ,// normalized

	const glm::vec2& centerB,
	const glm::vec2& halfExtentsB,
	const glm::vec2& axisBX,// normalized
	const glm::vec2& axisBZ// normalized
)
{
	// 4分離軸（Aの2軸 + Bの2軸）
	glm::vec2 axes[] = { axisAX, axisAZ, axisBX, axisBZ };

	// AからBへのベクトル
	glm::vec2 centerDiff = centerB - centerA;

	for (const glm::vec2& axis : axes)
	{
		// Aの投影半径
		float projA =
			halfExtentsA.x * std::abs(glm::dot(axis, axisAX)) +
			halfExtentsA.y * std::abs(glm::dot(axis, axisAZ));

		// Bの投影半径
		float projB =
			halfExtentsB.x * std::abs(glm::dot(axis, axisBX)) +
			halfExtentsB.y * std::abs(glm::dot(axis, axisBZ));

		// 両中心の距離をその軸に投影
		float centerProj = std::abs(glm::dot(centerDiff, axis));

		// 投影が分離していれば衝突していない
		if (centerProj > projA + projB)
			return false; // 分離軸が存在 → 衝突なし
	}

	return true; // すべての軸で投影が重なっている → 衝突あり
}

// 呼び出しは、`if(c.type == ColliderType::box2D) else {// 未定義動作}`
bool Engine::Physics::Logic2D::Collision::intersectBox2D(const Engine::ECS::Component::Logic2D::Box2D& a, const Engine::ECS::Component::Logic2D::Box2D& b)
{
	glm::vec2 aMin = a.center - a.halfExtents;
	glm::vec2 aMax = a.center + a.halfExtents;
	glm::vec2 bMin = b.center - b.halfExtents;
	glm::vec2 bMax = b.center + b.halfExtents;

	bool calc = (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
		(aMin.y <= bMax.y && aMax.y >= bMin.y);

	return calc;
}


bool Engine::Physics::Logic2D::Collision::CheckCircleAABBIntersection(
	const glm::vec2& circleCenter,
	float radius,
	const glm::vec2& boxMin,
	const glm::vec2& boxMax
)
{
	// AABB内で円の中心に最も近い点を求める
	float nearestX = glm::clamp(circleCenter.x, boxMin.x, boxMax.x);
	float nearestY = glm::clamp(circleCenter.y, boxMin.y, boxMax.y);

	// その点と円の中心の距離の２乗
	float dx = circleCenter.x - nearestX;
	float dy = circleCenter.y - nearestY;

	// この方法だと，rootを使うので浮動小数点誤差が拡大してしまう可能性がある
	//float distance = glm::distance(circleCenter, glm::vec2(nearestX, nearestY));
	//float distanceSquared = distance * distance;

	float squareSum = dx * dx + dy * dy;

	//std::cout << "distance^2: " << distanceSquared << "\n";
	//std::cout << "dx*dx + dy*dy: " << squareSum << "\n";
	//std::cout << "diff: " << std::abs(distanceSquared - squareSum) << "\n";
	// std::cout << "[CollisionUtils.cpp(CheckCircleAABBIntersection)]: result. " << i << std::endl;

	// 半径の２乗と比較して交差判定
	return squareSum <= (radius * radius);
}

// Separating Axis Theorem (SAT)
bool Engine::Physics::Logic2D::Collision::intersectOBB2D_AABB2D(const Engine::ECS::Component::Logic2D::Obb2D& obb, const glm::vec2& aabbMin, const glm::vec2& aabbMax)
{
	// AABBの中心とハーフサイズ
	glm::vec2 aabbCenter = 0.5f * (aabbMin + aabbMax);
	glm::vec2 aabbHalfExtents = 0.5f * (aabbMax - aabbMin);

	// AABBの軸(固定)
	glm::vec2 aabbAxes[2] = {
		glm::vec2(1.0f, 0.0f),// X軸
		glm::vec2(0.0f, 1.0f)// Z軸
	};

	glm::vec2 obbAxes[2] = {
		obb.axisX,
		obb.axisZ
	};

	for (int i = 0; i < 2; ++i)
	{
		glm::vec2 axis = aabbAxes[i];

		// AABBの投影半径
		float aabbProjection = glm::dot(aabbHalfExtents, glm::abs(axis));

		// OBBの投影半径
		float obbProjection =
			obb.halfExtents.x * std::abs(glm::dot(obb.axisX, axis)) +
			obb.halfExtents.y * std::abs(glm::dot(obb.axisZ, axis));

		// 中心間のベクトル
		glm::vec2 centerDiff = obb.center - aabbCenter;
		float centerDist = std::abs(glm::dot(centerDiff, axis));

		if (centerDist > aabbProjection + obbProjection)
			return false;
	}

	for (int i = 0; i < 2; ++i)
	{
		glm::vec2 axis = obbAxes[i];

		float aabbProj = aabbHalfExtents.x * std::abs(glm::dot(glm::vec2(1, 0), axis)) +
			aabbHalfExtents.y * std::abs(glm::dot(glm::vec2(0, 1), axis));

		float obbProj =
			obb.halfExtents.x * std::abs(glm::dot(obb.axisX, axis)) +
			obb.halfExtents.y * std::abs(glm::dot(obb.axisZ, axis));

		glm::vec2 centerDiff = obb.center - aabbCenter;
		float centerDist = std::abs(glm::dot(centerDiff, axis));

		if (centerDist > aabbProj + obbProj)
			return false;
	}


	return true;// 全軸で分離なし= 衝突
}