#ifndef __MUGGLE_MATH_UTILS_H__
#define __MUGGLE_MATH_UTILS_H__

#include "base.h"
#include "math/matrix4x4.h"

NS_MUGGLE_BEGIN

MG_DLL void SetMathUseGL(bool flag);
MG_DLL bool GetMathUseGL();

class MathUtils
{
public:
	template<typename T>
	static Matrix4x4<T> Translate(const Vector3<T>& v)
	{
		Matrix4x4<T> mat4 = Matrix4x4<T>(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			v.x, v.y, v.z, 1.0
			);

		if (GetMathUseGL())
		{
			mat4 = mat4.Transpose();
		}

		return mat4;
	}
	template<typename T>
	static Matrix4x4<T> Rotate(const Quaternion<T>& quat)
	{
		Matrix4x4<T> mat4;

		mat4.m[0][0] = (T)2 * quat.x*quat.x + (T)2 * quat.w*quat.w - (T)1;
		mat4.m[0][1] = (T)2 * quat.x*quat.y + (T)2 * quat.z*quat.w;
		mat4.m[0][2] = (T)2 * quat.x*quat.z - (T)2 * quat.y*quat.w;
		mat4.m[0][3] = (T)0;

		mat4.m[1][0] = (T)2 * quat.x*quat.y - (T)2 * quat.z*quat.w;
		mat4.m[1][1] = (T)2 * quat.y*quat.y + (T)2 * quat.w*quat.w - (T)1;
		mat4.m[1][2] = (T)2 * quat.y*quat.z + (T)2 * quat.x*quat.w;
		mat4.m[1][3] = (T)0;

		mat4.m[2][0] = (T)2 * quat.x*quat.z + (T)2 * quat.y*quat.w;
		mat4.m[2][1] = (T)2 * quat.y*quat.z - (T)2 * quat.x*quat.w;
		mat4.m[2][2] = (T)2 * quat.z*quat.z + (T)2 * quat.w*quat.w - (T)1;
		mat4.m[2][3] = (T)0;

		mat4.m[0][3] = (T)0;
		mat4.m[1][3] = (T)0;
		mat4.m[2][3] = (T)0;
		mat4.m[3][3] = (T)1;

		if (GetMathUseGL())
		{
			mat4 = mat4.Transpose();
		}

		return mat4;
	}
	template<typename T>
	static Matrix4x4<T> Scale(const Vector3<T>& v)
	{
		return Matrix4x4<T>(
			v.x, (T)0, (T)0, (T)0,
			(T)0, v.y, (T)0, (T)0
			(T)0, (T)0, v.z, (T)0
			(T)0, (T)0, (T)0, (T)1
			);
	}
	template<typename T>
	static Matrix4x4<T> TRS(const Vector3<T>& pos, const Quaternion<T>& quat, const Vector3<T>& scale)
	{
		if (GetMathUseGL())
		{
			return Translate(pos, use_gl) * Rotate(quat, use_gl) * Scale(scale);
		}
		return Scale(scale) * Rotate(quat) * Translate(pos);
	}

	template<typename T>
	static Matrix4x4<T> LookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
	{
		Vector3<T> forward, z_axis, x_axis, y_axis;

		forward = target - eye;
		forward.Normalize();

		y_axis = up;
		y_axis.Normalize();

		x_axis = y_axis.Cross(forward);
		x_axis.Normalize();

		z_axis = x_axis.Cross(y_axis);
		z_axis.Normalize();

		Matrix4x4<T> mat4 = Matrix4x4<T>(
			x_axis.x, y_axis.x, z_axis.x, (T)0,
			x_axis.y, y_axis.y, z_axis.y, (T)0,
			x_axis.z, y_axis.z, z_axis.z, (T)0,
			Vector3<T>::Dot(-eye, x_axis), Vector3<T>::Dot(-eye, y_axis), Vector3<T>::Dot(-eye, z_axis), (T)1
			);

		if (use_gl)
		{
			mat4 = mat4.Transpose();
		}

		return mat4;
	}
	template<typename T>
	static Matrix4x4<T> Ortho(const T& width, const T& height, const T& zNear, const T& zFar)
	{
		Matrix4x4<T> mat4 = Matrix4x4<T>::zero;

		mat4.m[0][0] = (T)2 / width;
		mat4.m[1][1] = (T)2 / height;
		mat4.m[3][3] = (T)1;
		if (GetMathUseGL())
		{
			mat4.m[2][2] = (T)(-2) / (zFar - zNear);
			mat4.m[2][3] = -(zFar + zNear) / (zFar - zNear);
		}
		else
		{
			mat4.m[2][2] = (T)1 / (zFar - zNear);
			mat4.m[3][2] = -zNear / (zFar - zNear);
		}		

		return mat4;
	}
	template<typename T>
	static Matrix4x4<T> Perspective(const T& fov, const T& aspect, const T& zNear, const T& zFar)
	{
		Matrix4x4<T> mat4 = Matrix4x4<T>::zero;

		/*
		T width = (T)2 * near * Math::Tan(fov * (T)0.5);
		T height = width / aspect;

		mat4.m[0][0] = (T)2 * near / width;
		mat4.m[1][1] = (T)2 * near / height;
		*/
		T width_optimize = Math::Tan(fov * (T)0.5);
		T height_optimize = width_optimize / aspect;

		mat4.m[0][0] = (T)1 / Math::Tan(fov * (T)0.5);
		mat4.m[1][1] = (T)1 / height_optimize;

		if (GetMathUseGL())
		{
			mat4.m[2][2] = -(zFar + zNear) / (zFar - zNear);
			mat4.m[2][3] = (T)(-2) * zFar * zNear / (zFar - zNear);
			mat4.m[3][2] = (T)1;
		}
		else
		{
			mat4.m[2][2] = zFar / (zFar - zNear);
			mat4.m[2][3] = (T)1;
			mat4.m[3][2] = -zFar * zNear / (zFar - zNear);
		}	

		return mat4;
	}
};

NS_MUGGLE_END

#endif