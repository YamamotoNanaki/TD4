#include "IFEMath.h"
#include <cassert>
#include <cmath>

using namespace IFE;

Float4 IFE::SetFloat4ToFloat3(const Float3& f, float w)
{
	return Float4(f.x, f.y, f.z, w);
}

Float4 IFE::SetFloat4ToFoat2(const Float2& f, float z, float w)
{
	return Float4(f.x, f.y, z, w);
}

Float3 IFE::SetFloat3ToFloat2(const Float2& f, float z)
{
	return Float3(f.x, f.y, z);
}

Float4 IFE::SetFloat4ToVector3(const Vector3& v, float w)
{
	return Float4(v.x, v.y, v.z, w);
}

Float3 IFE::SetFloat3(const Vector3& v)
{
	return Float3(v.x, v.y, v.z);
}

Float2 IFE::SetFloat3ToVector2(const Vector2& v)
{
	return Float2(v.x, v.y);
}

Vector3 IFE::SetVector3(const Float3& f)
{
	return Vector3(f.x, f.y, f.z);
}

Vector3 IFE::SetVector3ToFloat2(const Float2& f, float z)
{
	return Vector3(f.x, f.y, z);
}

Vector2 IFE::SetVector2(const Float2& f)
{
	return Vector2(f.x, f.y);
}

const Vector3 IFE::Vector3Zero()
{
	return Vector3(0, 0, 0);
}

bool IFE::Vector3Equal(const Vector3& v1, const Vector3& v2)
{
	return (((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z)) != 0);
}

float IFE::Vector3Length(const Vector3& v)
{
	return v.Length();
}
//
//Vector3& IFE::Vector3Normalize(Vector3& v)
//{
//	float length = v.Length();
//	if (length != 0)
//	{
//		Vector3 a = v / length;
//		return a;
//	}
//	return v;
//}

Vector3 IFE::Vector3Normalize(const Vector3& v)
{
	float length = v.Length();
	if (length != 0)
	{
		Vector3 a = v / length;
		return a;
	}
	return v;
}

float IFE::Vector3Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 IFE::Vector3Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 vec(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	return vec;
}

bool IFE::NearEqual(float S1, float S2, float epsilon)
{
	float Delta = S1 - S2;
	return (fabsf(Delta) <= epsilon);
}

bool IFE::NearEqual(const Float2& f1, const Float2& f2, float epsilon)
{
	return NearEqual(f1.x, f2.x, epsilon) && NearEqual(f1.y, f2.y, epsilon);
}

bool IFE::NearEqual(const Float3& f1, const Float3& f2, float epsilon)
{
	return NearEqual(f1.x, f2.x, epsilon) && NearEqual(f1.y, f2.y, epsilon) && NearEqual(f1.z, f2.z, epsilon);
}

void IFE::ScalarSinCos(float* pSin, float* pCos, float Value)
{
	assert(pSin);
	assert(pCos);

	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = static_cast<float>(M_PI_2) * Value;
	if (Value >= 0.0f)
	{
		quotient = static_cast<float>(static_cast<std::int32_t>(quotient + 0.5f));
	}
	else
	{
		quotient = static_cast<float>(static_cast<std::int32_t>(quotient - 0.5f));
	}
	float y = Value - static_cast<float>(M_PI) * 2 * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float sign;
	if (y > static_cast<float>(M_PI_2))
	{
		y = static_cast<float>(M_PI) - y;
		sign = -1.0f;
	}
	else if (y < -static_cast<float>(M_PI_2))
	{
		y = -static_cast<float>(M_PI) - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	// 11-degree minimax approximation
	*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*pCos = sign * p;
}

float IFE::ConvertToRadians(float fDegrees)
{
	return fDegrees * (static_cast<float>(M_PI) / 180.0f);
}

float IFE::ConvertToDegrees(float fRadians)
{
	return fRadians * (180.0f / static_cast<float>(M_PI));
}

Float3 IFE::ConvertToRadians(const Float3& fDegrees)
{
	return { ConvertToRadians(fDegrees.x),ConvertToRadians(fDegrees.y),ConvertToRadians(fDegrees.z) };
}

Float3 IFE::ConvertToDegrees(const Float3& fRadians)
{
	return { ConvertToDegrees(fRadians.x),ConvertToDegrees(fRadians.y),ConvertToDegrees(fRadians.z) };
}

Vector3 IFE::Vector3Transform(const Vector3& v, const Matrix& m)
{
	Vector3 mx = { m.m[0][0],m.m[0][1],m.m[0][2] };
	Vector3 my = { m.m[1][0],m.m[1][1],m.m[1][2] };
	Vector3 mz = { m.m[2][0],m.m[2][1],m.m[2][2] };
	Vector3 mw = { m.m[3][0],m.m[3][1],m.m[3][2] };

	Vector3 Result = {
		v.z * mz.x + mw.x,
		v.z * mz.y + mw.y,
		v.z * mz.z + mw.z };
	Result = Vector3(
		v.y * my.x + Result.x,
		v.y * my.y + Result.y,
		v.y * my.z + Result.z);
	Result = Vector3(
		v.x * mx.x + Result.x,
		v.x * mx.y + Result.y,
		v.x * mx.z + Result.z);

	return Result;
}

Vector3 IFE::Vector3TransformNormal(const Vector3& v, const Matrix& m)
{
	float Z = v.z;
	float Y = v.y;
	float X = v.x;

	Vector3 Result = { Z * m.m[2][0],Z * m.m[2][1],Z * m.m[2][2] };
	Result += { Y* m.m[1][0], Y* m.m[1][1], Y* m.m[1][2] };
	Result += { X* m.m[0][0], X* m.m[0][1], X* m.m[0][2] };

	return Result;
}

Vector3 IFE::VectorNegate(const Vector3& m)
{
	return -m;
}

Vector3 IFE::VectorLerp(const Vector3& v0, const Vector3& v1, float t)
{
	Vector3 length = VectorSubtract(v1, v0);
	return Vector3(length.x * t + v0.x, length.y * t + v0.y, length.z * t + v0.z);
}

Matrix IFE::MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
{
	assert(NearZ > 0.f && FarZ > 0.f);
	assert(!NearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
	assert(!NearEqual(AspectRatio, 0.0f, 0.00001f));
	assert(!NearEqual(FarZ, NearZ, 0.00001f));

	float    SinFov;
	float    CosFov;
	ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

	float Height = CosFov / SinFov;
	float Width = Height / AspectRatio;
	float fRange = FarZ / (FarZ - NearZ);

	return Matrix(Width, 0, 0, 0, 0, Height, 0, 0, 0, 0, fRange, 1, 0, 0, -fRange * NearZ, 0);
}

Float3 IFE::operator+(const Float3& f, const Vector2& v)
{
	return Float3(f.x + v.x, f.y + v.y, f.z);
}

float IFE::Average(Vector2 v)
{
	return (v.x + v.y) / 2;
}

float IFE::Average(Vector3 v)
{
	return (v.x + v.y + v.z) / 3;
}

float IFE::Complement(float& x1, float& x2, float flame)
{
	float distanceX = x2 - x1;//ãóó£ÇèoÇ∑
	float dividedDistanceX = distanceX / flame;//ãóó£ÇflameÇ≈äÑÇ¡ÇΩíl

	x1 += dividedDistanceX;//ãóó£ÇflameÇ≈äÑÇ¡ÇΩílÇë´Ç∑

	return x1;
}

float IFE::SimpleHarmonicMotion(float time, float amplitude, float period)
{
	return amplitude * sinf(2 * static_cast<float>(PI) * time / period);
}

float IFE::Vector3Max(const Vector3& vec)
{
	return fmaxf(vec.x, fmaxf(vec.y, vec.z));
}

float IFE::Vector3Min(const Vector3& vec)
{
	return fminf(vec.x, fminf(vec.y, vec.z));
}

Matrix IFE::Lerp(const Matrix& mat, float timer, float maxTime)
{
	timer = std::min(std::max(0.f, maxTime), 1.f) / maxTime;
	return mat * timer;
}

Matrix IFE::Lerp(const Matrix& mat1, const Matrix& mat2, float timer, float maxTime)
{
	timer = std::min(std::max(0.f, maxTime), 1.f) / maxTime;
	return mat1 * (1 - timer) + mat2 * timer;
}

int32_t IFE::Int2::operator=(const int32_t& i)
{
	return i;
}
