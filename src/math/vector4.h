#ifndef __MUGGLE_VECTOR4_H__
#define __MUGGLE_VECTOR4_H__

#include "base.h"
#include "math_base.h"
#include "vector3.h"

NS_MUGGLE_BEGIN

template<typename T>
struct Vector4_Integer
{
	T x;
	T y;
	T z;
	T w;

	explicit Vector4_Integer(const T& x_in = (T)0, const T& y_in = (T)0, const T& z_in = (T)0, const T& w_in = (T)1)
		: x(x_in)
		, y(y_in)
		, z(z_in)
		, w(w_in)
	{}
};

template<typename T>
struct Vector4
{
	T x;
	T y;
	T z;
	T w;

	explicit Vector4(const T& x_in = (T)0, const T& y_in = (T)0, const T& z_in = (T)0, const T& w_in = (T)1)
		: x(x_in)
		, y(y_in)
		, z(z_in)
		, w(w_in)
	{}
	explicit Vector4(const Vector3<T>& v)
		: x(v.x)
		, y(v.y)
		, z(v.z)
		, w((T)1)
	{}
	explicit Vector4(const Vector2<T>& v)
		: x(v.x)
		, y(v.y)
		, z((T)0)
		, w((T)1)
	{}
	explicit operator Vector3<T>()
	{
		return Vector3<T>(x, y, z);
	}
	explicit operator Vector2<T>()
	{
		return Vector2<T>(x, y);
	}

	bool operator==(const Vector4<T>& v) const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}
	bool operator!=(const Vector4<T>& v) const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}
	Vector4<T> operator-() const
	{
		return Vector4<T>(-x, -y, -z, -w);
	}
	Vector4<T> operator+(const Vector4<T>& v) const
	{
		return Vector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	Vector4<T> operator-(const Vector4<T>& v) const
	{
		return Vector4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	Vector4<T> operator*(const Vector4<T>& v) const
	{
		return Vector4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
	}
	Vector4<T> operator/(const Vector4<T>& v) const
	{
		return Vector4<T>(x / v.x, y / v.y, z / v.z, w / v.w);
	}
	Vector4<T> operator/(const T& scalar) const
	{
		return Vector4<T>(x / scalar, y / scalar, z / scalar, w / scalar);
	}
	Vector4<T> operator+=(const Vector4<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	Vector4<T> operator-=(const Vector4<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	Vector4<T> operator*=(const Vector4<T>& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	Vector4<T> operator/=(const Vector4<T>& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}
	Vector4<T> operator/=(const T& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}
	T& operator[](int index)
	{
		MASSERT(index >= 0 && index < 4);
		MASSERT(uintptr_t(&y) - uintptr_t(&x) == sizeof(T));
		MASSERT(uintptr_t(&z) - uintptr_t(&y) == sizeof(T));
		MASSERT(uintptr_t(&w) - uintptr_t(&z) == sizeof(T));
		return (&x)[index];
	}
	T operator[](int index) const
	{
		MASSERT(index >= 0 && index < 4);
		MASSERT(uintptr_t(&y) - uintptr_t(&x) == sizeof(T));
		MASSERT(uintptr_t(&z) - uintptr_t(&y) == sizeof(T));
		MASSERT(uintptr_t(&w) - uintptr_t(&z) == sizeof(T));
		return (&x)[index];
	}

	bool Equal(const Vector3<T>& v, T deviation = MIN_DEVIATION) const
	{
		return Math::Equal(x, v.x, deviation) &&
			Math::Equal(y, v.y, deviation) &&
			Math::Equal(z, v.z, deviation) &&
			Math::Equal(w, v.w, deviation);
	}
	T Length() const
	{
		return Math::Sqrt(x*x + y*y + z*z + w*w);
	}
	T SqrLength() const
	{
		return x*x + y*y + z*z + w*w;
	}
	T Normalize()
	{
		T len = Length();
		if (len > MIN_DEVIATION)
		{
			x /= len;
			y /= len;
			z /= len;
			w /= w;
			return len;
		}
		return (T)0;
	}

	T Dot(const Vector4<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	static T Dot(const Vector4<T>& v1, const Vector4<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}
};

template<typename T>
Vector4<T> operator*(const Vector4<T>& v, const T& scalar)
{
	return Vector4<T>(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}
template<typename T>
Vector4<T> operator*(const T& scalar, const Vector4<T>& v)
{
	return Vector4<T>(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

typedef Vector4_Integer<int> vec4i;
typedef Vector4_Integer<unsigned int> vec4u;
typedef Vector4_Integer<long> vec4l;
typedef Vector4_Integer<unsigned long> vec4ul;
typedef Vector4_Integer<int8_t> vec4_i8;
typedef Vector4_Integer<int16_t> vec4_i16;
typedef Vector4_Integer<int32_t> vec4_i32;
typedef Vector4_Integer<int64_t> vec4_i64;
typedef Vector4_Integer<uint8_t> vec4_u8;
typedef Vector4_Integer<uint16_t> vec4_u16;
typedef Vector4_Integer<uint32_t> vec4_u32;
typedef Vector4_Integer<uint64_t> vec4_u64;

typedef Vector4<float> vec4f;
typedef Vector4<double> vec4d;

NS_MUGGLE_END

#endif