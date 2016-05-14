#ifndef __MUGGLE_QUATERNION_H__
#define __MUGGLE_QUATERNION_H__

#include "base.h"
#include "math_base.h"
#include "vector3.h"

NS_MUGGLE_BEGIN

template<typename T>
struct Quaternion
{
	T x;
	T y;
	T z;
	T w;

	Quaternion(const T& w_in = (T)0, const T& x_in = (T)0, const T& y_in = (T)0, const T& z_in = (T)0)
		: x(x_in)
		, y(y_in)
		, z(z_in)
		, w(w_in)
	{}

	static Quaternion<T> FromYawPitchRoll(const T& yaw, const T& pitch, const T& roll)
	{
		Quaternion<T> quat;

		T half_yaw = yaw / (T)2;
		T half_pitch = pitch / (T)2;
		T half_roll = roll / (T)2;

		quat.w = 
			Math::Cos(half_yaw) * Math::Cos(half_pitch) * Math::Cos(half_roll) + 
			Math::Sin(half_yaw) * Math::Sin(half_pitch) * Math::Sin(half_roll);
		quat.x = 
			Math::Cos(half_yaw) * Math::Sin(half_pitch) * Math::Cos(half_roll) +
			Math::Sin(half_yaw) * Math::Cos(half_pitch) * Math::Sin(half_roll);
		quat.y =
			Math::Sin(half_yaw) * Math::Cos(half_pitch) * Math::Cos(half_roll) -
			Math::Cos(half_yaw) * Math::Sin(half_pitch) * Math::Sin(half_roll);
		quat.z =
			Math::Cos(half_yaw) * Math::Cos(half_pitch) * Math::Sin(half_roll) -
			Math::Sin(half_yaw) * Math::Sin(half_pitch) * Math::Cos(half_roll);

		return quat;
	}

	Quaternion<T> operator+ (const Quaternion<T>& q) const
	{
		return Quaternion<T>(w + q.w, x + q.x, y + q.y, z + q.z);
	}
	Quaternion<T> operator- (const Quaternion<T>& q) const
	{
		return Quaternion<T>(w - q.w, x - q.x, y - q.y, z - q.z);
	}
	Quaternion<T> operator* (const Quaternion<T>& q) const
	{
		return Quaternion<T>(
			w*q.w - x*q.x - y*q.y - z*q.z,
			w*q.x + x*q.w + y*q.z - z*q.y,
			w*q.y + y*q.w + z*q.x - x*q.z,
			w*q.z + z*q.w + x*q.y - y*q.x
		);
	}
	Quaternion<T> operator* (T scalar) const
	{
		return Quaternion<T>(scalar * w, scalar * x, scalar * y, scalar * z);
	}
	Vector3<T> operator*(const Vector3<T>& v)
	{
		Vector3<T> u(x, y, z);
		T s = w;

		Vector3<T> ret_v = (T)2 * u.Dot(v) * u
			+ (s * s - u.Dot(u)) * v
			+ 2.0f * s * u.Cross(v);
		return ret_v;
	}

	T Quaternion::Normalize()
	{
		T sqr_len = w*w + x*x + y*y + z*z;
		T len = Math::Sqrt(sqr_len);
		if (Math::Equal(len, (T)0))
		{
			return T(0);
		}
		T inv_sqrt_len = (T)1 / len;
		*this = *this * inv_sqrt_len;
		return len;
	}
};

typedef Quaternion<float> quatf;

NS_MUGGLE_END

#endif