#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <cmath>

namespace Engine::Math
{
	template<typename T, std::size_t N>
	struct Vec;	// àÍî å`ÇÕñ¢íËã`(2/3/4ÇæÇØégÇ§)

	template<typename T>
	struct Vec<T, 2>
	{
		static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
		T x{};
		T y{};

		constexpr Vec() = default;
		constexpr Vec(T x_, T y_): x(x_), y(y_){}
		constexpr Vec(const Vec& v_) = default;

		constexpr Vec& operator+=(const Vec& o) noexcept { x += o.x; y += o.y; return *this; }
		constexpr Vec& operator-=(const Vec& o) noexcept { x -= o.x; y -= o.y; return *this; }
		constexpr Vec& operator*=(const T s) noexcept { x *= s; y *= s; return *this; }
		Vec& operator/=(const T s) { assert(s != T(0)); x /= s; y /= s; return *this; }

		friend constexpr Vec operator+(Vec a, const Vec& b) noexcept { return a += b; }
		friend constexpr Vec operator-(Vec a, const Vec& b) noexcept { return a -= b; }
		friend constexpr Vec operator*(Vec a, const T s) noexcept { return a *= s; }
		friend constexpr Vec operator*(T s, Vec a) noexcept { return a *= s; }
		friend Vec operator/(Vec a, const T s) { return a /= s; }

		// unary minus
		friend constexpr Vec operator-(Vec v) noexcept
		{
			v.x = -v.x;
			v.y = -v.y;
			return v;
		}

		static constexpr Vec Zero() noexcept { return Vec{ T(0), T(0) }; }
		static constexpr Vec One() noexcept { return Vec{ T(1), T(1) }; }
	};


	template<typename T>
	struct Vec<T, 3>
	{
		static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
		T x{};
		T y{};
		T z{};

		constexpr Vec() = default;
		constexpr Vec(T x_, T y_, T z_): x(x_), y(y_), z(z_){}

		constexpr Vec& operator+=(const Vec& o) noexcept { x += o.x; y += o.y; z += o.z; return *this; }
		constexpr Vec& operator-=(const Vec& o) noexcept { x -= o.x; y -= o.y; z -= o.z; return *this; }
		constexpr Vec& operator*=(const T s) noexcept { x *= s; y *= s; z *= s; return *this; }
		Vec& operator/=(const T s) { assert(s != T(0)); x /= s; y /= s; z /= s; return *this; }

		friend constexpr Vec operator+(Vec a, const Vec& b) noexcept { return a += b; }
		friend constexpr Vec operator-(Vec a, const Vec& b) noexcept { return a -= b; }
		friend constexpr Vec operator*(Vec a, const T s) noexcept { return a *= s; }
		friend constexpr Vec operator*(T s, Vec a) noexcept { return a *= s; }
		friend Vec operator/(Vec a, const T s) { return a /= s; }

		// unary minus
		friend constexpr Vec operator-(Vec v) noexcept
		{
			v.x = -v.x;
			v.y = -v.y;
			v.z = -v.z;
			return v;
		}

		static constexpr Vec Zero() noexcept { return Vec{ T(0), T(0), T(0) }; }
		static constexpr Vec One() noexcept { return Vec{ T(1), T(1), T(1) }; }
	};

	template<typename T>
	struct Vec<T, 4>
	{
		static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
		T x{};
		T y{};
		T z{};
		T w{};

		constexpr Vec() = default;
		constexpr Vec(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}

		constexpr Vec& operator+=(const Vec& o) noexcept { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
		constexpr Vec& operator-=(const Vec& o) noexcept { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
		constexpr Vec& operator*=(const T s) noexcept { x *= s; y *= s; z *= s; w *= s; return *this; }
		Vec& operator/=(const T s) { assert(s != T(0)); x /= s; y /= s; z /= s; w /= s; return *this; }

		friend constexpr Vec operator+(Vec a, const Vec& b) noexcept { return a += b; }
		friend constexpr Vec operator-(Vec a, const Vec& b) noexcept { return a -= b; }
		friend constexpr Vec operator*(Vec a, const T s) noexcept { return a *= s; }
		friend constexpr Vec operator*(T s, Vec a) noexcept { return a *= s; }
		friend Vec operator/(Vec a, const T s) { return a /= s; }

		// unary minus
		friend constexpr Vec operator-(Vec v) noexcept
		{
			v.x = -v.x;
			v.y = -v.y;
			v.z = -v.z;
			v.w = -v.w;
			return v;
		}

		static constexpr Vec Zero() noexcept { return Vec{ T(0), T(0), T(0), T(0) }; }
		static constexpr Vec One() noexcept { return Vec{ T(1), T(1), T(1), T(1) }; }

	};

	using Vec2f = Vec<float, 2>;
	using Vec3f = Vec<float, 3>;
	using Vec4f = Vec<float, 4>;

	// Quaternion ÇÕ (x,y,z,w) Ç≈ï€éù (Engineë§ãKñÒ)
	template<typename T>
	struct Quat
	{
		static_assert(std::is_floating_point_v<T>);
		T x{ T(0) };
		T y{ T(0) };
		T z{ T(0) };
		T w{ T(1) };

		constexpr Quat() = default;
		constexpr Quat(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
		static constexpr Quat Identity() noexcept { return Quat{ T(0), T(0), T(0), T(1) }; }
	};

	using Quatf = Quat<float>;

	struct TRS
	{
		Vec3f translation = Vec3f::Zero();
		Quatf rotation = Quatf::Identity();
		Vec3f scale = Vec3f::One();
	};

	static Quatf FromYaw(float yawRad)
	{
		float half = yawRad * 0.5f;
		return
		{
			std::cos(half),
			0.0f,
			std::sin(half),
			0.0f
		};
	}

	// 
	void NormalizeInPlace(Quatf& q)noexcept;
}