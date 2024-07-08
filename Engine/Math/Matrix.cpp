#include "Matrix.h"
#include <cmath>
#include <cfloat>
#include <cassert>
#include <stdexcept>

using namespace IFE;

IFE::Matrix::Matrix()
{
	*this = MatrixIdentity();
}

IFE::Matrix::Matrix(float _1_1, float _1_2, float _1_3, float _1_4,
	float _2_1, float _2_2, float _2_3, float _2_4,
	float _3_1, float _3_2, float _3_3, float _3_4,
	float _4_1, float _4_2, float _4_3, float _4_4)
{
	m[0][0] = _1_1;  m[0][1] = _1_2;  m[0][2] = _1_3;  m[0][3] = _1_4;
	m[1][0] = _2_1;  m[1][1] = _2_2;  m[1][2] = _2_3;  m[1][3] = _2_4;
	m[2][0] = _3_1;  m[2][1] = _3_2;  m[2][2] = _3_3;  m[2][3] = _3_4;
	m[3][0] = _4_1;  m[3][1] = _4_2;  m[3][2] = _4_3;  m[3][3] = _4_4;
}

Matrix IFE::Matrix::operator+() const
{
	return *this;
}

Matrix IFE::Matrix::operator-() const
{
	return Matrix(-m[0][0], -m[0][1], -m[0][2], -m[0][3], -m[1][0], -m[1][1], -m[1][2], -m[1][3], -m[2][0], -m[2][1], -m[2][2], -m[2][3], -m[3][0], -m[3][1], -m[3][2], -m[3][3]);
}

Matrix IFE::Matrix::operator*(float s)const
{
	Matrix a;
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			a.m[i][j] = m[i][j] * s;
		}
	}

	return a;
}

Matrix IFE::Matrix::operator/(float s)const
{
	Matrix a;
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			a.m[i][j] = m[i][j] / s;
		}
	}

	return a;
}

Matrix& IFE::Matrix::operator+=(const Matrix& _m)
{
	*this = *this + _m;
	return *this;
}

Matrix& IFE::Matrix::operator-=(const Matrix& _m)
{
	*this = *this - _m;
	return *this;
}

Matrix& IFE::Matrix::operator*=(const Matrix& _m)
{
	*this = *this * _m;
	return *this;
}

Matrix& IFE::Matrix::operator*=(float s)
{
	*this = *this * s;
	return *this;
}

Matrix& IFE::Matrix::operator/=(float s)
{
	*this = *this / s;
	return *this;
}

Float4 IFE::Matrix::operator[](size_t row)
{
	if (row < 0 || row >= 4) {
		throw std::out_of_range("Index out of range");
	}

	return Float4(m[row][0], m[row][1], m[row][2], m[row][3]);
}

const Float4 IFE::Matrix::operator[](size_t row) const
{
	if (row < 0 || row >= 4) {
		throw std::out_of_range("Index out of range");
	}

	return Float4(m[row][0], m[row][1], m[row][2], m[row][3]);
}

void IFE::Matrix::SetX(float x, float y, float z, float w)
{
	m[0][0] = x;
	m[0][1] = y;
	m[0][2] = z;
	m[0][3] = w;
}

void IFE::Matrix::SetY(float x, float y, float z, float w)
{
	m[1][0] = x;
	m[1][1] = y;
	m[1][2] = z;
	m[1][3] = w;
}

void IFE::Matrix::SetZ(float x, float y, float z, float w)
{
	m[2][0] = x;
	m[2][1] = y;
	m[2][2] = z;
	m[2][3] = w;
}

void IFE::Matrix::SetW(float x, float y, float z, float w)
{
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	m[3][3] = w;
}

void IFE::Matrix::SetX(Vector3 v, float w)
{
	m[0][0] = v.x;
	m[0][1] = v.y;
	m[0][2] = v.z;
	m[0][3] = w;
}

void IFE::Matrix::SetY(Vector3 v, float w)
{
	m[1][0] = v.x;
	m[1][1] = v.y;
	m[1][2] = v.z;
	m[1][3] = w;
}

void IFE::Matrix::SetZ(Vector3 v, float w)
{
	m[2][0] = v.x;
	m[2][1] = v.y;
	m[2][2] = v.z;
	m[2][3] = w;
}

void IFE::Matrix::SetW(Vector3 v, float w)
{
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
	m[3][3] = w;
}

Matrix IFE::Matrix::Inverse()
{
	return MatrixInverse(*this);
}

void IFE::GetArrayMatrix(const Matrix& matrix, float arrayPtr[16])
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			arrayPtr[i * 4 + j] = matrix.m[i][j];
		}
	}
}

void IFE::SetArrayMatrix(Matrix& matrix, float arrayPtr[])
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			matrix.m[i][j] = arrayPtr[i * 4 + j];
		}
	}
}

Matrix IFE::MatrixIdentity()
{
	return Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix IFE::MatrixInverse(const Matrix& m)
{
	Matrix mat = m;
	Matrix inv;
	float buf;
	for (int32_t i = 0; i < 4; i++) {
		if (mat.m[i][i] == 0)
		{
			mat.m[i][i] = FLT_EPSILON;
		}
		buf = 1 / mat.m[i][i];
		for (int32_t j = 0; j < 4; j++) {
			mat.m[i][j] *= buf;
			inv.m[i][j] *= buf;
		}
		for (int32_t j = 0; j < 4; j++) {
			if (i != j) {
				buf = mat.m[j][i];
				for (int32_t k = 0; k < 4; k++) {
					mat.m[j][k] -= mat.m[i][k] * buf;
					inv.m[j][k] -= inv.m[i][k] * buf;
				}
			}
		}
	}

	return inv;
}

Matrix IFE::MatrixTranspose(const Matrix& m)
{
	return Matrix(
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

Matrix IFE::MatrixScaling(float sx, float sy, float sz)
{
	return Matrix(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
}

Matrix IFE::MatrixRotationX(float angle)
{
	float fSinAngle = sinf(angle);
	float fCosAngle = cosf(angle);
	return Matrix(1, 0, 0, 0, 0, fCosAngle, fSinAngle, 0, 0, -fSinAngle, fCosAngle, 0, 0, 0, 0, 1);
}

Matrix IFE::MatrixRotationY(float angle)
{
	float fSinAngle = sinf(angle);
	float fCosAngle = cosf(angle);
	return Matrix(fCosAngle, 0, -fSinAngle, 0, 0, 1, 0, 0, fSinAngle, 0, fCosAngle, 0, 0, 0, 0, 1);
}

Matrix IFE::MatrixRotationZ(float angle)
{
	float fSinAngle = sinf(angle);
	float fCosAngle = cosf(angle);
	return Matrix(fCosAngle, fSinAngle, 0, 0, -fSinAngle, fCosAngle, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix IFE::MatrixRotation(float x, float y, float z, float angle)
{
	float cs = cosf(angle);
	float si = sinf(angle);
	Matrix a;
	a.m[0][0] = cs + x * x * (1 - cs);
	a.m[0][1] = x * y * (1 - cs) - z * si;
	a.m[0][2] = z * y * (1 - cs) + x * si;
	a.m[1][0] = x * y * (1 - cs) - z * si;
	a.m[1][1] = cs + y * y * (1 - cs);
	a.m[1][2] = z * y * (1 - cs) - x * si;
	a.m[2][0] = z * x * (1 - cs) - y * si;
	a.m[2][1] = z * y * (1 - cs) + x * si;
	a.m[2][2] = cs + z * z * (1 - cs);

	a.m[0][3] = 0;
	a.m[1][3] = 0;
	a.m[2][3] = 0;
	a.m[3][0] = 0;
	a.m[3][1] = 0;
	a.m[3][2] = 0;
	a.m[3][3] = 1;

	return a;
}

Matrix IFE::MatrixTranslation(float tx, float ty, float tz)
{
	return Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
}

Matrix IFE::MatrixOrthoGraphicProjection(float left, float right, float top, float bottom, float near, float far)
{
	float width = 2.0f / (right - left);
	float height = -2.0f / (bottom - top);
	float zRange = 1.0f / (far - near);
	return Matrix(width, 0, 0, 0, 0, height, 0, 0, 0, 0, zRange, 0, -1, 1, 0, 1);
}

Matrix IFE::operator+(const Matrix& m1, const Matrix& m2)
{
	Matrix a;

	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			a.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

	return a;
}

Matrix IFE::operator-(const Matrix& m1, const Matrix& m2)
{
	Matrix a;
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			a.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}

	return a;
}

Matrix IFE::operator*(const Matrix& m1, const Matrix& m2)
{
	Matrix a;
	a.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	a.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	a.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	a.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	a.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	a.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	a.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	a.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	a.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	a.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	a.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	a.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	a.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	a.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	a.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	a.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	return a;
}

Float3 IFE::GetScale(const Matrix& m)
{
	Vector3 x = { m.m[0][0],m.m[0][1],m.m[0][2] };
	Vector3 y = { m.m[1][0],m.m[1][1],m.m[1][2] };
	Vector3 z = { m.m[2][0],m.m[2][1],m.m[2][2] };

	Float3 f = {x.Length(),y.Length(),z.Length()};

	return f;
}

//座標変換（ベクトルと行列の掛け算をする）
Vector3 Matrix::Transform(const Vector3 v, const Matrix& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};
	if (std::isnan(result.x)
		|| std::isnan(result.y)
		|| std::isnan(result.z))
	{
		assert(0);
	}
	return result;
}
