#ifndef __MUGGLE_VECTOR2_H__
#define __MUGGLE_VECTOR2_H__

#include "base.h"
#include "math_utility.h"

NS_MUGGLE_BEGIN

template<typename T>
struct Vector2_Integer
{
	T x;
	T y;

	explicit Vector2_Integer(const T& x_in = (T)0, const T& y_in = (T)0)
		: x(x_in)
		, y(y_in)
	{}
};

template<typename T>
struct Vector2
{
	T x;
	T y;

	const static Vector2<T> zero;
	const static Vector2<T> left;
	const static Vector2<T> right;
	const static Vector2<T> up;
	const static Vector2<T> down;

	explicit Vector2(const T& x_in = (T)0, const T& y_in = (T)0)
		: x(x_in)
		, y(y_in)
	{}

	bool operator==(const Vector2<T>& v) const
	{
		return x == v.x && y == v.y;
	}
	bool operator!=(const Vector2<T>& v) const
	{
		return x != v.x || y != v.y;
	}
	Vector2<T> operator+(const Vector2<T>& v) const
	{
		return Vector2<T>(x + v.x, y + v.y);
	}
	Vector2<T> operator-(const Vector2<T>& v) const
	{
		return Vector2<T>(x - v.x, y - v.y);
	}
	Vector2<T> operator*(const Vector2<T>& v) const
	{
		return Vector2<T>(x * v.x, y * v.y);
	}
	Vector2<T> operator/(const Vector2<T>& v) const
	{
		return Vector2<T>(x / v.x, y / v.y);
	}
	Vector2<T> operator/(const T& scalar) const
	{
		return Vector2<T>(x / scalar, y / scalar);
	}
	Vector2<T> operator+=(const Vector2<T>& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2<T> operator-=(const Vector2<T>& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vector2<T> operator*=(const Vector2<T>& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	Vector2<T> operator/=(const Vector2<T>& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	Vector2<T> operator/=(const T& scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}
	T& operator[](int index)
	{
		MASSERT(index >= 0 && index < 2);
		MASSERT(uintptr_t(&y) - uintptr_t(&x) == sizeof(T));
		return (&x)[index];
	}
	T operator[](int index) const
	{
		MASSERT(index >= 0 && index < 2);
		MASSERT(uintptr_t(&y) - uintptr_t(&x) == sizeof(T));
		return (&x)[index];
	}

	bool Equal(const Vector2<T>& v, T deviation = MIN_DEVIATION) const
	{
		return Math::Equal(x, v.x, deviation) && Math::Equal(y, v.y, deviation);
	}
	T Length() const
	{
		return Math::Sqrt(x*x + y*y);
	}
	T SqrLength() const
	{
		return x*x + y*y;
	}
	T Normalize()
	{
		T len = Length();
		if (len > MIN_DEVIATION)
		{
			x /= len;
			y /= len;
			return len;
		}		
		return (T)0;
	}

	static T Dot(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}
	static T Cross(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}
	/*
	 *  Return angle in radian
	 *  Return value in the range[-PI, PI];
	 */
	static T Angle(const Vector2<T>& from, const Vector2<T>& to)
	{
		Vector2<T> from_normal = from;
		Vector2<T> to_normal = to;
		T from_len = from_normal.Normalize();
		T to_len = to_normal.Normalize();
		if (Math::Equal(from_len, (T)0) || Math::Equal(to_len, (T)0))
		{
			return (T)0;
		}

		T cos_radian = Dot(from_normal, to_normal);
		T arccos = Math::ACos(cos_radian);

		// <0 represent clockwise, >0 represent counterclockwise
		T sin_radian = Cross(from_normal, to_normal);
		T cw = (T)(sin_radian < 0 ? 1 : -1);

		return cw * arccos;
	}

};

template<typename T>
const Vector2<T> Vector2<T>::zero = Vector2<T>((T)0, (T)0);
template<typename T>
const Vector2<T> Vector2<T>::left = Vector2<T>((T)-1, (T)0);
template<typename T>
const Vector2<T> Vector2<T>::right = Vector2<T>((T)1, (T)0);
template<typename T>
const Vector2<T> Vector2<T>::up = Vector2<T>((T)0, (T)1);
template<typename T>
const Vector2<T> Vector2<T>::down = Vector2<T>((T)0, (T)-1);

template<typename T>
Vector2<T> operator*(const Vector2<T>& v, const T& scalar)
{
	return Vector2<T>(v.x * scalar, v.y * scalar);
}
template<typename T>
Vector2<T> operator*(const T& scalar, const Vector2<T>& v)
{
	return Vector2<T>(v.x * scalar, v.y * scalar);
}

typedef Vector2_Integer<int> vec2i;
typedef Vector2_Integer<unsigned int> vec2u;
typedef Vector2_Integer<long> vec2l;
typedef Vector2_Integer<unsigned long> vec2ul;
typedef Vector2_Integer<int8_t> vec2_i8;
typedef Vector2_Integer<int16_t> vec2_i16;
typedef Vector2_Integer<int32_t> vec2_i32;
typedef Vector2_Integer<int64_t> vec2_i64;
typedef Vector2_Integer<uint8_t> vec2_u8;
typedef Vector2_Integer<uint16_t> vec2_u16;
typedef Vector2_Integer<uint32_t> vec2_u32;
typedef Vector2_Integer<uint64_t> vec2_u64;

typedef Vector2<float> vec2f;
typedef Vector2<double> vec2d;

NS_MUGGLE_END

#endif