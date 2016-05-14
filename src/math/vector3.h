#ifndef __MUGGLE_VECTOR3_H__
#define __MUGGLE_VECTOR3_H__

#include "base.h"
#include "math_base.h"
#include "vector2.h"

NS_MUGGLE_BEGIN

template<typename T>
struct Vector3_Integer
{
	T x;
	T y;
	T z;

	explicit Vector3_Integer(const T& x_in = (T)0, const T& y_in = (T)0, const T& z_in = (T)0)
		: x(x_in)
		, y(y_in)
		, z(z_in)
	{}
};

template<typename T>
struct Vector3
{
	T x;
	T y;
	T z;

	const static Vector3<T> zero;
	const static Vector3<T> left;
	const static Vector3<T> right;
	const static Vector3<T> up;
	const static Vector3<T> down;
	const static Vector3<T> forward;
	const static Vector3<T> backward;

	explicit Vector3(const T& x_in = (T)0, const T& y_in = (T)0, const T& z_in = (T)0)
		: x(x_in)
		, y(y_in)
		, z(z_in)
	{}
	explicit Vector3(const Vector2<T>& v)
		: x(v.x)
		, y(v.y)
		, z((T)0)
	{}
	explicit operator Vector2<T>()
	{
		return Vector2<T>(x, y);
	}

	bool operator==(const Vector3<T>& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator!=(const Vector3<T>& v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}
	Vector3<T> operator-() const
	{
		return Vector3<T>(-x, -y, -z);
	}
	Vector3<T> operator+(const Vector3<T>& v) const
	{
		return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}
	Vector3<T> operator-(const Vector3<T>& v) const
	{
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}
	Vector3<T> operator*(const Vector3<T>& v) const
	{
		return Vector3<T>(x * v.x, y * v.y, z * v.z);
	}
	Vector3<T> operator/(const Vector3<T>& v) const
	{
		return Vector3<T>(x / v.x, y / v.y, z / v.z);
	}
	Vector3<T> operator/(const T& scalar) const
	{
		return Vector3<T>(x / scalar, y / scalar, z / scalar);
	}
	Vector3<T> operator+=(const Vector3<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3<T> operator-=(const Vector3<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	Vector3<T> operator*=(const Vector3<T>& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector3<T> operator/=(const Vector3<T>& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	Vector3<T> operator/=(const T& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
	T& operator[](int index)
	{
		MASSERT(index >= 0 && index < 3);
		MASSERT(uintptr_t(&y) - uintptr_t(&x) == sizeof(T));
		MASSERT(uintptr_t(&z) - uintptr_t(&y) == sizeof(T));
		return (&x)[index];
	}
	T operator[](int index) const
	{
		MASSERT(index >= 0 && index < 3);
		MASSERT(uintptr_t(&y) - uintptr_t(&x) == sizeof(T));
		MASSERT(uintptr_t(&z) - uintptr_t(&y) == sizeof(T));
		return (&x)[index];
	}

	bool Equal(const Vector3<T>& v, T deviation = MIN_DEVIATION) const
	{
		return Math::Equal(x, v.x, deviation) && 
			Math::Equal(y, v.y, deviation) &&
			Math::Equal(z, v.z, deviation);
	}
	T Length() const
	{
		return Math::Sqrt(x*x + y*y + z*z);
	}
	T SqrLength() const
	{
		return x*x + y*y + z*z;
	}
	T Normalize()
	{
		T len = Length();
		if (len > MIN_DEVIATION)
		{
			x /= len;
			y /= len;
			z /= len;
			return len;
		}
		return (T)0;
	}

	T Dot(const Vector3<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vector3<T> Cross(const Vector3<T>& v) const
	{
		return Vector3<T>(
			y*v.z - z*v.y,
			z*v.x - x*v.z,
			x*v.y - y*v.x
			);
	}

	static T Dot(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static Vector3<T> Cross(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(
				v1.y*v2.z - v1.z*v2.y,
				v1.z*v2.x - v1.x*v2.z,
				v1.x*v2.y - v1.y*v2.x
			);
	}
	/*
	 *  Return angle in radian
	 *  Return value in the range[0, PI];
	 */
	static T Angle(const Vector3<T>& from, const Vector3<T>& to)
	{
		Vector3<T> from_normal = from;
		Vector3<T> to_normal = to;
		T from_len = from_normal.Normalize();
		T to_len = to_normal.Normalize();
		if (Math::Equal(from_len, (T)0) || Math::Equal(to_len, (T)0))
		{
			return (T)0;
		}

		T cos_radian = Dot(from_normal, to_normal);
		T arccos = Math::ACos(cos_radian);

		return arccos;
	}

	/*
	 *  convert between Spherical Coordinates and Cartesian Coordinates
	 *  note: in convention, z is up, but this is not compliance
	 *			this use: y is up, x is right, z is forward, left hand
	 *  theta in range [0, pi]
	 *  phi in range[0, 2*pi]
	 */
	static Vector3<T> FromSpherical(const T& theta, const T& phi)
	{
		theta = Math::Clamp(theta, (T)0, (T)PI);
		phi = Math::Clamp(phi, (T)0, (T)(PI*2));

		return Vector3<T>(
			cos(phi)*sin(theta),
			cos(theta),
			sin(phi)*sin(theta)
			);
	}
	static Vector2<T> ToSpherical()
	{
		T len = Normalize();
		if (Math::Equal(len, (T)0))
		{
			return Vector2<T>::zero;
		}

		T theta = Math::ACos(y / Length());
		T phi = Math::ATan2(z, x);
		phi = phi < (T)0 ? phi + (T)(2 * PI) : phi;
		return Vector2<T>(theta, phi);
	}
};

template<typename T>
const Vector3<T> Vector3<T>::zero = Vector3<T>((T)0, (T)0, (T)0);
template<typename T>
const Vector3<T> Vector3<T>::left = Vector3<T>((T)-1, (T)0, (T)0);
template<typename T>
const Vector3<T> Vector3<T>::right = Vector3<T>((T)1, (T)0, (T)0);
template<typename T>
const Vector3<T> Vector3<T>::up = Vector3<T>((T)0, (T)1, (T)0);
template<typename T>
const Vector3<T> Vector3<T>::down = Vector3<T>((T)0, (T)-1, (T)0);
template<typename T>
const Vector3<T> Vector3<T>::forward = Vector3<T>((T)0, (T)0, (T)1);
template<typename T>
const Vector3<T> Vector3<T>::backward = Vector3<T>((T)0, (T)0, (T)-1);

template<typename T>
Vector3<T> operator*(const Vector3<T>& v, const T& scalar)
{
	return Vector3<T>(v.x * scalar, v.y * scalar, v.z * scalar);
}
template<typename T>
Vector3<T> operator*(const T& scalar, const Vector3<T>& v)
{
	return Vector3<T>(v.x * scalar, v.y * scalar, v.z * scalar);
}

typedef Vector3_Integer<int> vec3i;
typedef Vector3_Integer<unsigned int> vec3u;
typedef Vector3_Integer<long> vec3l;
typedef Vector3_Integer<unsigned long> vec3ul;
typedef Vector3_Integer<int8_t> vec3_i8;
typedef Vector3_Integer<int16_t> vec3_i16;
typedef Vector3_Integer<int32_t> vec3_i32;
typedef Vector3_Integer<int64_t> vec3_i64;
typedef Vector3_Integer<uint8_t> vec3_u8;
typedef Vector3_Integer<uint16_t> vec3_u16;
typedef Vector3_Integer<uint32_t> vec3_u32;
typedef Vector3_Integer<uint64_t> vec3_u64;

typedef Vector3<float> vec3f;
typedef Vector3<double> vec3d;

NS_MUGGLE_END

#endif