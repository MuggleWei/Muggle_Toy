#ifndef __MUGGLE_MATH_BASE_H__
#define __MUGGLE_MATH_BASE_H__

#include "base.h"
#include <float.h>

NS_MUGGLE_BEGIN

#undef  PI
#define PI 					(3.141593)
#define INV_PI				(0.31830988618)
#define HALF_PI				(1.57079632679)
#define MIN_DEVIATION		(FLT_EPSILON)

class Math
{
public:

	template<typename T>
	static T DegreeToRad(const T& degree_val)
	{
		return degree_val * (T)PI / (T)180;
	}

	template<typename T>
	static bool Equal(const T& val1, const T& val2, T deviation = MIN_DEVIATION)
	{
		return Math::Abs(val1 - val2) <= deviation;
	}

	template<typename T>
	static T Abs(const T& val)
	{
		return (val >= (T)0) ? val : -val;
	}

	template<typename T>
	static T Clamp(const T& val, const T& min, const T& max)
	{
		MASSERT_MSG(min < max, "This may be not you want");
		return val > max ? max : (val < min ? min : val);
	}

	template<typename T>
	static T Sqrt(const T& val)
	{
		return sqrtf(val);
	}

	template<typename T>
	static T Sin(const T& val)
	{
		return sinf(val);
	}
	template<typename T>
	static T ASin(const T& val)
	{
		return asinf(Clamp(val, (T)-1, (T)1));
	}
	template<typename T>
	static T Cos(const T& val)
	{
		return cosf(val);
	}
	template<typename T>
	static T ACos(const T& val)
	{
		return acosf(Clamp(val, (T)-1, (T)1));
	}
	template<typename T>
	static T Tan(const T& val)
	{
		return tan(val);
		// return tanf(val);
	}
	template<typename T>
	static T ATan(const T& val)
	{
		return atan(val);
	}
	template<typename T>
	static T ATan2(const T& y, const T& x)
	{
		return atan2f(y, x);
	}
};

NS_MUGGLE_END

#endif