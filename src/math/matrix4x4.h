#ifndef __MUGGLE_MATRIX4X4_H__
#define __MUGGLE_MATRIX4X4_H__

#include "base.h"
#include "math_utility.h"
#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"

NS_MUGGLE_BEGIN

template<typename T>
struct Matrix4x4
{
	// Matrix elements are m[row][column]
	union
	{
		T mat[16];
		T m[4][4];
	};

	static const Matrix4x4<T> identify;
	static const Matrix4x4<T> zero;

	explicit Matrix4x4(
		const T& m00 = (T)1, const T& m01 = (T)0, const T& m02 = (T)0, const T& m03 = (T)0,
		const T& m10 = (T)0, const T& m11 = (T)1, const T& m12 = (T)0, const T& m13 = (T)0,
		const T& m20 = (T)0, const T& m21 = (T)0, const T& m22 = (T)1, const T& m23 = (T)0,
		const T& m30 = (T)0, const T& m31 = (T)0, const T& m32 = (T)0, const T& m33 = (T)1
		)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	Matrix4x4<T> operator*(const Matrix4x4<T>& matrix)
	{
		Matrix4x4<T> new_mat;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				new_mat[i][j] = m[i][0] * matrix[0][j] + m[i][1] * matrix[1][j] + m[i][2] * matrix[2][j] + m[i][3] * matrix[3][j];
			}
		}

		return new_mat;
	}

	Vector4<T> GetRow(int i) const
	{
		MASSERT(index >= 0 && index < 4);
		return Vector4<T>(m[i][0], m[i][1], m[i][2], m[i][3]);
	}
	Vector4<T> GetColumn(int i) const
	{
		MASSERT(index >= 0 && index < 4);
		return Vector4<T>(m[0][i], m[1][i], m[2][i], m[3][i]);
	}
	void SetRow(int i, const Vector4<T>& v)
	{
		MASSERT(index >= 0 && index < 4);
		m[i][0] = v.x;
		m[i][1] = v.y;
		m[i][2] = v.z;
		m[i][3] = v.w;
	}
	void SetColumn(int i, const Vector4<T>& v)
	{
		MASSERT(index >= 0 && index < 4);
		m[0][i] = v.x;
		m[1][i] = v.y;
		m[2][i] = v.z;
		m[3][i] = v.w;
	}

	T Cofactor(int row, int col) const
	{
		int r0 = 0 >= row ? 1 : 0;
		int r1 = 1 >= row ? 2 : 1;
		int r2 = 2 >= row ? 3 : 2;
		int c0 = 0 >= col ? 1 : 0;
		int c1 = 1 >= col ? 2 : 1;
		int c2 = 2 >= col ? 3 : 2;

		return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r1][c2] * m[r2][c1]) +
			m[r0][c1] * (m[r1][c2] * m[r2][c0] - m[r1][c0] * m[r2][c2]) +
			m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r1][c0] * m[r2][c0]);
	}
	T Determinant() const
	{
		return mat[0]*Cofactor(0,0) - mat[1]*Cofactor(0,1) + mat[2]*Cofactor(0,2) - mat[3]*Cofactor(0,3);
	}
	Matrix4x4<T> Adjugate() const
	{
		Matrix4x4<T> mat4_adj =
		{
			Cofactor(0, 0), (T)-1 * Cofactor(0, 1), Cofactor(0, 2), (T)-1 * Cofactor(0, 3),
			(T)-1 * Cofactor(1, 0), Cofactor(1, 1), (T)-1 * Cofactor(1, 2), Cofactor(1, 3),
			Cofactor(2, 0), (T)-1 * Cofactor(2, 1), Cofactor(2, 2), (T)-1 * Cofactor(2, 3),
			(T)-1 * Cofactor(3, 0), Cofactor(3, 1), (T)-1 * Cofactor(3, 2), Cofactor(3, 3)
		};

		return mat4_adj;
	}
	bool Inverse(Matrix4x4<T>& inv_mat) const
	{
		T determinant = Determinant();

		if (Math::Equal(determinant, T(0)))
		{
			inv_mat = Matrix4x4<T>::identify;
			return false;
		}

		T inv_det = (T)1 / determinant;
		if (Math::Equal(inv_det, T(0)))
		{
			inv_mat = Matrix4x4<T>::identify;
			return false;
		}

		Matrix4x4<T> mat4_adj = Adjugate();
		inv_mat = mat4_adj * inv_det;

		return true;
	}
	Matrix4x4<T> Transpose() const
	{
		Matrix4x4<T> mat_transpose;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat_transpose.m[i][j] = m[j][i];
			}
		}

		return mat_transpose;
	}

	Vector4<T> Multiply(const Vector4<T>& v)
	{
		return Vector4<T>(
			v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
			v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
			v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
			v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]
		);
	}

	// utility method
	static Matrix4x4<T> Translate(const Vector3<T>& v)
	{
		return Matrix4x4<T>(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			v.x, v.y, v.z, 1.0
		);
	}
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

		return mat4;
	}
	static Matrix4x4<T> Scale(const Vector3<T>& v)
	{
		return Matrix4x4<T>(
			v.x, (T)0, (T)0, (T)0, 
			(T)0, v.y, (T)0, (T)0
			(T)0, (T)0, v.z, (T)0
			(T)0, (T)0, (T)0, (T)1
		);
	}
	static Matrix4x4<T> TRS(const Vector3<T>& pos, const Quaternion<T>& quat, const Vector3<T>& scale)
	{
		return Matrix4x4<T>::Scale(scale) * Matrix4x4<T>::Rotate(quat) * Matrix4x4<T>::Translate(pos);
	}

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

		return Matrix4x4<T>(
			x_axis.x, y_axis.x, z_axis.x, (T)0,
			x_axis.y, y_axis.y, z_axis.y, (T)0,
			x_axis.z, y_axis.z, z_axis.z, (T)0,
			Vector3<T>::Dot(-eye, x_axis), Vector3<T>::Dot(-eye, y_axis), Vector3<T>::Dot(-eye, z_axis), (T)1
		);
	}
	static Matrix4x4<T> Ortho(const T& width, const T& height, const T& zNear, const T& zFar)
	{
		Matrix4x4<T> mat4 = Matrix4x4<T>::zero;
		mat4.m[0][0] = (T)2 / width;
		mat4.m[1][1] = (T)2 / height;
		mat4.m[2][2] = (T)1 / (zFar - zNear);
		mat4.m[3][2] = -zNear / (zFar - zNear);
		mat4.m[3][3] = (T)1;

		return mat4;
	}
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
		mat4.m[2][2] = zFar / (zFar - zNear);
		mat4.m[2][3] = (T)1;
		mat4.m[3][2] = -zFar * zNear / (zFar - zNear);

		return mat4;
	}
	
};

template<typename T>
const Matrix4x4<T> Matrix4x4<T>::identify = Matrix4x4<T>(
	(T)1, (T)0, (T)0, (T)0,
	(T)0, (T)1, (T)0, (T)0,
	(T)0, (T)0, (T)1, (T)0,
	(T)0, (T)0, (T)0, (T)1
	);

template<typename T>
const Matrix4x4<T> Matrix4x4<T>::zero = Matrix4x4<T>(
	(T)0, (T)0, (T)0, (T)0,
	(T)0, (T)0, (T)0, (T)0,
	(T)0, (T)0, (T)0, (T)0,
	(T)0, (T)0, (T)0, (T)0
	);

template<typename T>
Matrix4x4<T> operator*(const Matrix4x4<T>& mat4, const T& scalar)
{
	return Matrix4x4<T>(
		mat4.m[0][0] * scalar, mat4.m[0][1] * scalar, mat4.m[0][2] * scalar, mat4.m[0][3] * scalar,
		mat4.m[1][0] * scalar, mat4.m[1][1] * scalar, mat4.m[1][2] * scalar, mat4.m[1][3] * scalar,
		mat4.m[2][0] * scalar, mat4.m[2][1] * scalar, mat4.m[2][2] * scalar, mat4.m[2][3] * scalar,
		mat4.m[3][0] * scalar, mat4.m[3][1] * scalar, mat4.m[3][2] * scalar, mat4.m[3][3] * scalar
		);
}
template<typename T>
Matrix4x4<T> operator*(const T& scalar, const Matrix4x4<T>& mat4)
{
	return Matrix4x4<T>(
		mat4.m[0][0] * scalar, mat4.m[0][1] * scalar, mat4.m[0][2] * scalar, mat4.m[0][3] * scalar,
		mat4.m[1][0] * scalar, mat4.m[1][1] * scalar, mat4.m[1][2] * scalar, mat4.m[1][3] * scalar,
		mat4.m[2][0] * scalar, mat4.m[2][1] * scalar, mat4.m[2][2] * scalar, mat4.m[2][3] * scalar,
		mat4.m[3][0] * scalar, mat4.m[3][1] * scalar, mat4.m[3][2] * scalar, mat4.m[3][3] * scalar
		);
}

typedef Matrix4x4<float> matrix4f;
typedef Matrix4x4<double> matrix4d;

NS_MUGGLE_END

#endif