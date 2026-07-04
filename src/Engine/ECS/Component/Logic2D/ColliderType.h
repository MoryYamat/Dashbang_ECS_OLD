// Collision shape local definition **Use in conjunction with world transformation info when used**
// コリジョンの形状ローカル定義 **使用時にワールド変換情報と組み合わせて使う**
// 
#pragma once

#include <GLM/glm.hpp>

#include <cmath>



#include <variant>
#include <cassert>

namespace Engine::ECS::Component::Logic2D
{
	// name of ColliderType
	enum class ColliderType
	{
		None,
		Circle2D,
		Box2D,
		Obb2D,
		AABB3D,
		Sphere3D,
		Capsule3D,
	};

	// 2D円
	struct Circle2D
	{
		// ローカルオフセット
		glm::vec2 center;
		float radius;
	};

	// 2D正方形
	struct Box2D
	{
		// ローカルオフセット
		glm::vec2 center;
		glm::vec2 halfExtents;
	};

	// Oriented Bounding Box
	// 回転あり境界ボックス
	struct Obb2D
	{
		// ローカルオフセット
		glm::vec2 center;//
		glm::vec2 halfExtents; // (width/2, height/2)
		glm::vec2 axisX; // X軸方向の単位ベクトル
		glm::vec2 axisZ;// axisに直交

		// 点が含まれるかどうかを判定
		bool contains(const glm::vec2& pointXZ) const
		{
			// 相対ベクトル
			glm::vec2 d = pointXZ - center;
			float dx = glm::dot(d, axisX);// ローカルx軸への投影
			float dz = glm::dot(d, axisZ);// ローカルz軸への投影

			// 投影の絶対値とモデル半径(XZそれぞれを)比較して判定
			return std::abs(dx) <= halfExtents.x && std::abs(dz) <= halfExtents.y;
		}
	};



	using CollideVariant = std::variant<std::monostate, Circle2D, Box2D, Obb2D>;

	// Collider Data Structure
	struct Collider
	{
		//Engine::ECS::Component::Logic2D::ColliderType type;

		// パフォーマンスと安全性を高める方法を考えるまではVariantで
		CollideVariant shape;

		ColliderType getType() const
		{
			return std::visit([](auto&& arg) -> ColliderType {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, Circle2D>) return ColliderType::Circle2D;
				else if constexpr (std::is_same_v<T, Box2D>) return ColliderType::Box2D;
				else if constexpr (std::is_same_v<T, Obb2D>) return ColliderType::Obb2D;
				else return ColliderType::None;
				}, shape);
		}

		// helper functions
		bool IsCircle2D() const { return std::holds_alternative<Circle2D>(shape); }
		bool IsBox2D() const { return std::holds_alternative<Box2D>(shape); }
		bool IsObb2D() const { return std::holds_alternative<Obb2D>(shape); }

		Circle2D& AsCircle2D() { return std::get<Circle2D>(shape); }
		Box2D& AsBox2D() { return std::get<Box2D>(shape); }
		Obb2D& AsObb2D() { return std::get<Obb2D>(shape); }

		const Circle2D& AsCircle2D() const 
		{ 
			assert(IsCircle2D());
			return std::get<Circle2D>(shape); 
		}
		const Box2D& AsBox2D() const 
		{
			assert(IsBox2D());
			return std::get<Box2D>(shape); 
		}
		const Obb2D& AsObb2D() const 
		{
			assert(IsObb2D());// 開発時のみチェック
			return std::get<Obb2D>(shape); 
		}
		
	};

}

//struct AABB3D
//{
//	// ローカルオフセット
//	glm::vec3 min;
//	glm::vec3 max;
//};

//struct Sphere3D
//{
//	// ローカルオフセット
//	glm::vec3 center;
//	float radius;
//};

//struct Capsule3D
//{
//	// ローカルオフセット
//	glm::vec3 base;
//	glm::vec3 tip;
//	float radius;
//};

//// 2D
//enum class Collider2DType
//{
//	None,
//	Circle2D,
//	Box2D,
//};
//
//struct Collider2D
//{
//	Collider2DType type;
//	union
//	{
//		struct { glm::vec2 center; float radius; } circle;
//		struct { glm::vec2 center; glm::vec2 halfExtents; } box;
//	};
//};
//
//
//// 3D
//enum class Collider3DType
//{
//	None,
//	AABB3D,
//	Sphere3D,
//	Capsule3D,
//	Mesh3D,
//};
//
//struct Collider3DType
//{
//	Collider3DType type;
//	union
//	{
//		struct { glm::vec3 min; glm::vec3 max; } aabb;
//		struct { glm::vec3 center; float radius; } sphere;
//	};
//};


//memo
// Quadtree
// Octree
// Bounding volume hierarchy(BVH)
// Grid Hashing, Layer Musk, Broad Fase, narrow fase
//