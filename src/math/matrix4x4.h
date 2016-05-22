#ifndef __MUGGLE_MATRIX4X4_H__
#define __MUGGLE_MATRIX4X4_H__

#include "base.h"
#include "math_base.h"
#include "vector3.h"
#include "vector4.h"
#include "quaternion.h"
#include "matrix3x3.h"

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
	explicit operator Matrix3x3<T>()
	{
		return Matrix3x3<T>(
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]
			);
	}

	Matrix4x4<T> operator*(const Matrix4x4<T>& matrix)
	{
		Matrix4x4<T> new_mat;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				new_mat.m[i][j] = m[i][0] * matrix.m[0][j] + m[i][1] * matrix.m[1][j] + m[i][2] * matrix.m[2][j] + m[i][3] * matrix.m[3][j];
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
		Matrix4x4<T> mat4_adj = Matrix4x4<T>(
			Cofactor(0, 0), (T)-1 * Cofactor(1, 0), Cofactor(2, 0), (T)-1 * Cofactor(3, 0),
			(T)-1 * Cofactor(0, 1), Cofactor(1, 1), (T)-1 * Cofactor(2, 1), Cofactor(3, 1),
			Cofactor(0, 2), (T)-1 * Cofactor(1, 2), Cofactor(2, 2), (T)-1 * Cofactor(3, 2),
			(T)-1 * Cofactor(0, 3), Cofactor(1, 3), (T)-1 * Cofactor(2, 3), Cofactor(3, 3)
		);

		return mat4_adj;
	}
	Matrix4x4<T> Inverse(bool *succeed = nullptr) const
	{
		T determinant = Determinant();
		Matrix4x4<T> inv_mat;

		if (Math::Equal(determinant, T(0)))
		{
			inv_mat = Matrix4x4<T>::identify;
			if (succeed)
			{
				*succeed = false;
			}			
			return inv_mat;
		}

		T inv_det = (T)1 / determinant;
		if (Math::Equal(inv_det, T(0)))
		{
			inv_mat = Matrix4x4<T>::identify;
			if (succeed)
			{
				*succeed = false;
			}
			return inv_mat;
		}

		Matrix4x4<T> mat4_adj = Adjugate();
		inv_mat = mat4_adj * inv_det;

		if (succeed)
		{
			*succeed = false;
		}
		return inv_mat;
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

	static Matrix4x4<T> Inverse(const Matrix4x4<T>& mat)
	{
		return mat.Inverse();
	}
	static Matrix4x4<T> Transpose(const Matrix4x4<T>& mat)
	{
		return mat.Transpose();
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