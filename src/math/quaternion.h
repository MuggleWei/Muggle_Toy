#ifndef __MUGGLE_QUATERNION_H__
#define __MUGGLE_QUATERNION_H__

#include "base.h"
#include "math_utility.h"

NS_MUGGLE_BEGIN

template<typename T>
struct Quaternion
{
	T x;
	T y;
	T z;
	T w;

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
};

typedef Quaternion<float> Quat;

NS_MUGGLE_END

#endif