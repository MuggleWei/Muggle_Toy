#ifndef __MUGGLE_MATRIX3X3_H__
#define __MUGGLE_MATRIX3X3_H__

#include "base.h"
#include "math_base.h"
#include "vector3.h"
#include "quaternion.h"

NS_MUGGLE_BEGIN

template<typename T>
struct Matrix3x3
{
	// Matrix elements are m[row][column]
	union
	{
		T mat[9];
		T m[3][3];
	};

	static const Matrix3x3<T> identify;
	static const Matrix3x3<T> zero;

	explicit Matrix3x3(
		const T& m00 = (T)1, const T& m01 = (T)0, const T& m02 = (T)0,
		const T& m10 = (T)0, const T& m11 = (T)1, const T& m12 = (T)0,
		const T& m20 = (T)0, const T& m21 = (T)0, const T& m22 = (T)1
	)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
	}

	Matrix3x3<T> operator*(const Matrix3x3<T>& matrix)
	{
		Matrix3x3<T> new_mat;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				new_mat.m[i][j] = m[i][0] * matrix.m[0][j] + m[i][1] * matrix.m[1][j] + m[i][2] * matrix.m[2][j];
			}
		}

		return new_mat;
	}

	Vector3<T> GetRow(int i) const
	{
		MASSERT(index >= 0 && index < 3);
		return Vector3<T>(m[i][0], m[i][1], m[i][2]);
	}
	Vector3<T> GetColumn(int i) const
	{
		MASSERT(index >= 0 && index < 3);
		return Vector3<T>(m[0][i], m[1][i], m[2][i]);
	}
	void SetRow(int i, const Vector3<T>& v)
	{
		MASSERT(index >= 0 && index < 3);
		m[i][0] = v.x;
		m[i][1] = v.y;
		m[i][2] = v.z;
	}
	void SetColumn(int i, const Vector3<T>& v)
	{
		MASSERT(index >= 0 && index < 3);
		m[0][i] = v.x;
		m[1][i] = v.y;
		m[2][i] = v.z;
	}

	T Cofactor(int row, int col) const
	{
		int r0 = 0 >= row ? 1 : 0;
		int r1 = 1 >= row ? 2 : 1;
		int c0 = 0 >= col ? 1 : 0;
		int c1 = 1 >= col ? 2 : 1;

		return m[r0][c0] * m[r1][c1] - m[r0][c1] * m[r1][c0];
	}
	T Determinant() const
	{
		return mat[0] * Cofactor(0, 0) - mat[1] * Cofactor(0, 1) + mat[2] * Cofactor(0, 2);
	}
	Matrix3x3<T> Adjugate() const
	{
		Matrix3x3<T> mat3_adj = Matrix3x3<T>(
			mat[4] * mat[8] - mat[5] * mat[7], mat[2] * mat[7] - mat[1] * mat[8], mat[1] * mat[5] - mat[2] * mat[4],
			mat[5] * mat[6] - mat[3] * mat[8], mat[0] * mat[8] - mat[2] * mat[6], mat[2] * mat[3] - mat[0] * mat[5],
			mat[3] * mat[7] - mat[4] * mat[6], mat[1] * mat[6] - mat[0] * mat[7], mat[0] * mat[4] - mat[1] * mat[3]
//			mat[4] * mat[8] - mat[5] * mat[7],	// row 0
//			mat[5] * mat[6] - mat[3] * mat[8],
//			mat[3] * mat[7] - mat[4] * mat[6],
//			mat[2] * mat[7] - mat[1] * mat[8],	// row 1
//			mat[0] * mat[8] - mat[2] * mat[6],
//			mat[1] * mat[6] - mat[0] * mat[7],
//			mat[1] * mat[5] - mat[2] * mat[4],	// row 2
//			mat[2] * mat[3] - mat[0] * mat[5],
//			mat[0] * mat[4] - mat[1] * mat[3]
		);

		return mat3_adj;
	}
	Matrix3x3<T> Inverse(bool *succeed = nullptr) const
	{
		T determinant = Determinant();
		Matrix3x3<T> inv_mat;

		if (Math::Equal(determinant, T(0)))
		{
			inv_mat = Matrix3x3<T>::identify;
			if (succeed)
			{
				*succeed = false;
			}
			return inv_mat;
		}

		T inv_det = (T)1 / determinant;
		if (Math::Equal(inv_det, T(0)))
		{
			inv_mat = Matrix3x3<T>::identify;
			if (succeed)
			{
				*succeed = false;
			}
			return inv_mat;
		}

		Matrix3x3<T> mat3_adj = Adjugate();
		inv_mat = mat3_adj * inv_det;

		if (succeed)
		{
			*succeed = false;
		}

		return inv_mat;
	}
	Matrix3x3<T> Transpose() const
	{
		Matrix3x3<T> mat_transpose;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				mat_transpose.m[i][j] = m[j][i];
			}
		}

		return mat_transpose;
	}

	Vector3<T> Multiply(const Vector3<T>& v)
	{
		return Vector3<T>(
			v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
			v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
			v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]
			);
	}

	static Matrix3x3<T> Inverse(const Matrix3x3<T>& mat)
	{
		return mat.Inverse();
	}
	static Matrix3x3<T> Transpose(const Matrix3x3<T>& mat)
	{
		return mat.Transpose();
	}
};

template<typename T>
const Matrix3x3<T> Matrix3x3<T>::identify = Matrix3x3<T>(
	(T)1, (T)0, (T)0,
	(T)0, (T)1, (T)0,
	(T)0, (T)0, (T)1
);

template<typename T>
const Matrix3x3<T> Matrix3x3<T>::zero = Matrix3x3<T>(
	(T)0, (T)0, (T)0,
	(T)0, (T)0, (T)0,
	(T)0, (T)0, (T)0
);

template<typename T>
Matrix3x3<T> operator*(const Matrix3x3<T>& mat3, const T& scalar)
{
	return Matrix3x3<T>(
		mat3.m[0][0] * scalar, mat3.m[0][1] * scalar, mat3.m[0][2] * scalar,
		mat3.m[1][0] * scalar, mat3.m[1][1] * scalar, mat3.m[1][2] * scalar,
		mat3.m[2][0] * scalar, mat3.m[2][1] * scalar, mat3.m[2][2] * scalar
		);
}
template<typename T>
Matrix3x3<T> operator*(const T& scalar, const Matrix3x3<T>& mat3)
{
	return Matrix3x3<T>(
		mat3.m[0][0] * scalar, mat3.m[0][1] * scalar, mat3.m[0][2] * scalar,
		mat3.m[1][0] * scalar, mat3.m[1][1] * scalar, mat3.m[1][2] * scalar,
		mat3.m[2][0] * scalar, mat3.m[2][1] * scalar, mat3.m[2][2] * scalar
		);
}

typedef Matrix3x3<float> matrix3f;
typedef Matrix3x3<double> matrix3d;

NS_MUGGLE_END

#endif