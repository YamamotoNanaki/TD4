#include "Quaternion.h"
#include "IFEMath.h"
#include <cmath>

using namespace IFE;

Quaternion IFE::quaternion(float x, float y, float z, float w)
{
	Quaternion r = { x,y,z,w };
	return r;
}

Quaternion IFE::quaternion(const Vector3& v, float angle)
{
	float s = sin(angle / 2.0f);
	return quaternion(s * v.x, s * v.y, s * v.z, cos(angle / 2.0f));
}

float IFE::Dot(const Quaternion& q1, const Quaternion& q2)
{
	float cos = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	return cos;
}

float IFE::Length(const Quaternion& q)
{
	return (float)sqrt(Dot(q, q));
}

Quaternion IFE::normalize(const Quaternion& q)
{
	Quaternion r = q;
	float l = Length(r);
	if (l != 0)r /= l;
	return r;
}

Quaternion IFE::Conjugate(const Quaternion& q)
{
	Quaternion a = { -q.x, -q.y, -q.z, q.w };
	return a;
}

Quaternion IFE::operator+(const Quaternion& q)
{
	return q;
}

Quaternion IFE::operator-(const Quaternion& q)
{
	Quaternion r = { -q.x,-q.y,-q.z,-q.w };
	return r;
}

Quaternion IFE::operator+=(Quaternion& q1, const Quaternion& q2)
{
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	q1.w += q2.w;
	return q1;
}

Quaternion IFE::operator-=(Quaternion& q1, const Quaternion& q2)
{
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	q1.w -= q2.w;
	return q1;
}

Quaternion IFE::operator*=(Quaternion& q1, const Quaternion& q2)
{
	Quaternion q = {
		 q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
		-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
		 q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
		-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w
	};
	q1 = q;
	return q1;
}

Quaternion IFE::operator*=(Quaternion& q, float s)
{
	q.x *= s;
	q.y *= s;
	q.z *= s;
	q.w *= s;
	return q;
}

Quaternion IFE::operator/=(Quaternion& q, float s)
{
	return q *= 1.0f / s;
}

Quaternion IFE::operator+(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion r = q1;
	return r += q2;
}

Quaternion IFE::operator-(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion r = q1;
	return r -= q2;
}

Quaternion IFE::operator*(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion r = q1;
	return r *= q2;
}

Quaternion IFE::operator*(const Quaternion& q, float s)
{
	Quaternion r = q;
	return r *= s;
}

Quaternion IFE::operator*(float s, const Quaternion& q)
{
	Quaternion r = q;
	return r *= s;
}

Quaternion IFE::operator/(const Quaternion& q1, float s)
{
	Quaternion r = q1;
	return r /= s;
}

Quaternion IFE::SLerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float c = Dot(q1, q2);
	Quaternion t2 = q2;
	if (c < 0.0f)
	{
		c = -c;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - c) > 0.001f)
	{
		float th = (float)acos(c);
		k0 = (float)(sin(th * k0) / sin(th));
		k1 = (float)(sin(th * k1) / sin(th));
	}
	return q1 * k0 + t2 * k1;
}

Quaternion IFE::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float c = Dot(q1, q2);
	Quaternion t2 = q2;
	if (c < 0.0f)
	{
		c = -c;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;

	return q1 * k0 + t2 * k1;
}

Matrix IFE::RotateMatrix(const Quaternion& q)
{
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float ww = q.w * q.w;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;
	return Matrix(
		ww + xx - yy - zz, 2 * (xy + wz), 2 * (xz - wy), 0.0f,
		2 * (xy - wz), ww - xx + yy - zz, 2 * (yz + wx), 0.0f,
		2 * (xz + wy), 2 * (yz - wx), ww - xx - yy + zz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Quaternion IFE::quaternion(const Matrix& m)
{
	Quaternion r;
	float t = m.m[0][0] + m.m[1][1] + m.m[2][2] + m.m[3][3];

	if (t >= 1.0f)
	{
		float f = 2.0f * sqrt(t);
		r.x = (m.m[1][2] - m.m[2][1]) / f;
		r.y = (m.m[2][0] - m.m[0][2]) / f;
		r.x = (m.m[0][1] - m.m[1][0]) / f;
		r.x = f / 4.0f;
		return r;
	}
	int32_t i = 0;
	if (m.m[0][0] <= m.m[1][1])i = 1;
	if (m.m[2][2] <= m.m[i][i])i = 2;
	int32_t j = (i + 1) % 3;
	int32_t k = (j + 1) % 3;
	t = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;
	float D = 2.0f * sqrt(t);
	float q[4];
	q[i] = D / 4.0f;
	q[j] = (m.m[j][i] + m.m[i][j]) / D;
	q[k] = (m.m[k][i] + m.m[i][k]) / D;
	q[3] = (m.m[j][k] - m.m[k][j]) / D;
	r.x = q[0];
	r.y = q[1];
	r.z = q[2];
	r.w = q[3];
	return r;
}

Vector3 IFE::GetAxis(const Quaternion& q)
{
	Vector3 r;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float l = Length(q);

	r.x = x / l;
	r.y = y / l;
	r.z = z / l;

	return r;
}

float IFE::GetRadian(const Quaternion& q)
{
	return 2 * acos(q.w);
}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion ret;

	ret = {
			(lhs.y * rhs.z) - (lhs.z * rhs.y) + (rhs.w * lhs.x) + (lhs.w * rhs.x),
			(lhs.z * rhs.x) - (lhs.x * rhs.z) + (rhs.w * lhs.y) + (lhs.w * rhs.y),
			(lhs.x * rhs.y) - (lhs.y * rhs.x) + (rhs.w * lhs.z) + (lhs.w * rhs.z),
		(lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z)
	};

	return ret;
}

Quaternion IFE::MakeAxisAngle(const Vector3& axis, float angle)
{
	Quaternion ret;
	Vector3 normalizedAxis = axis;
	normalizedAxis = normalizedAxis.Normalize();

	ret = { normalizedAxis.x * sinf(angle / 2.0f),normalizedAxis.y * sinf(angle / 2.0f),normalizedAxis.z * sinf(angle / 2.0f),cosf(angle / 2.0f) };

	return ret;
}

Quaternion IFE::EulerToQuaternion(const Float3& euler)
{
	Quaternion ret;

	//ZŽ²‰ñ“]s—ñ‚ðéŒ¾
	Quaternion matRotZ = MakeAxisAngle({ 0,0,1 }, euler.z);
	//XŽ²‰ñ“]s—ñ‚ðéŒ¾
	Quaternion matRotX = MakeAxisAngle({ 1,0,0 }, euler.x);
	//YŽ²‰ñ“]s—ñ‚ðéŒ¾
	Quaternion matRotY = MakeAxisAngle({ 0,1,0 }, euler.y);

	//‡¬—p‰ñ“]s—ñ‚ðéŒ¾‚µAZXY‚Ì‡‚É‡¬
	ret = Multiply(Multiply(matRotY, matRotX), matRotZ);

	return ret;
}

Float3 IFE::QuaternionToEuler(const Quaternion& q)
{
	float q0q0 = q.x * q.x;
	float q0q1 = q.x * q.y;
	float q0q2 = q.x * q.z;
	float q0q3 = q.x * q.w;
	float q1q1 = q.y * q.y;
	float q1q2 = q.y * q.z;
	float q1q3 = q.y * q.w;
	float q2q2 = q.z * q.z;
	float q2q3 = q.z * q.w;
	float q3q3 = q.w * q.w;
	float roll = atan2(2.0f * (q2q3 + q0q1), q0q0 - q1q1 - q2q2 + q3q3);
	float pitch = asin(2.0f * (q0q2 - q1q3));
	float yaw = atan2(2.0f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3);
	return Float3(roll, pitch, yaw);
}

Vector3 IFE::MultiplyQuaternionAndVector3(const Quaternion& rotation, const Vector3& point)
{
	float num = rotation.x * 2.f;
	float num2 = rotation.y * 2.f;
	float num3 = rotation.z * 2.f;
	float num4 = rotation.x * num;
	float num5 = rotation.y * num2;
	float num6 = rotation.z * num3;
	float num7 = rotation.x * num2;
	float num8 = rotation.x * num3;
	float num9 = rotation.y * num3;
	float num10 = rotation.w * num;
	float num11 = rotation.w * num2;
	float num12 = rotation.w * num3;
	Vector3 result;
	result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
	result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
	result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
	return result;
}

Quaternion IFE::DirectionToDirection(const Vector3& u, const Vector3& v)
{
	Vector3 nu = u;
	Vector3 nv = v;
	nu.Normalize();
	nv.Normalize();
	float dot = Vector3Dot(nu, nv);
	Vector3 cross = Vector3Cross(nu, nv);
	Vector3 axis = Vector3Normalize(cross);
	float theta = acos(dot);
	return MakeAxisAngle(axis, theta);
}

Float4 IFE::Quaternion::GetFloat4()
{
	return Float4(x,y,z,w);
}

void IFE::Quaternion::SetFloat4(const Float4& f)
{
	x = f.x;
	y = f.y;
	z = f.z;
	w = f.w;
}
