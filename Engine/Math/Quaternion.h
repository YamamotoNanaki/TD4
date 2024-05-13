#pragma once
#include "Vector3.h"
#include "Float4.h"
#include "Matrix.h"

namespace IFE
{
	struct Quaternion
	{
		float x, y, z, w;
		static Quaternion identity() { return Quaternion(0, 0, 0, 1); };
		Float4 GetFloat4();
		void SetFloat4(const Float4& f);
	};
	Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	Quaternion quaternion(const Vector3& v, float angle);
	float Dot(const Quaternion& q1, const Quaternion& q2);
	float Length(const Quaternion& q);
	Quaternion normalize(const Quaternion& q);
	Quaternion Conjugate(const Quaternion& q);

	Quaternion operator+ (const Quaternion& q);
	Quaternion operator- (const Quaternion& q);

	Quaternion operator+= (Quaternion& q1, const Quaternion& q2);
	Quaternion operator-= (Quaternion& q1, const Quaternion& q2);
	Quaternion operator*= (Quaternion& q1, const Quaternion& q2);
	Quaternion operator*= (Quaternion& q, float s);
	Quaternion operator/= (Quaternion& q, float s);

	Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator*(const Quaternion& q, float s);
	Quaternion operator*(float s, const Quaternion& q);
	Quaternion operator/(const Quaternion& q, float s);

	Quaternion SLerp(const Quaternion& q1, const Quaternion& q2, float t);
	Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
	Matrix RotateMatrix(const Quaternion& q);
	Quaternion quaternion(const Matrix& m);
	Vector3 GetAxis(const Quaternion& q);
	/// <summary>
	/// 角度求める
	/// </summary>
	/// <param name="q">クオタニオン</param>
	/// <returns>rad</returns>
	float GetRadian(const Quaternion& q);

	Quaternion MakeAxisAngle(const Vector3& axis, float angle);

	Quaternion EulerToQuaternion(const Float3& eulerDegrees);
	Float3 QuaternionToEuler(const Quaternion& q);

	Vector3 MultiplyQuaternionAndVector3(const Quaternion& rotation, const Vector3& point);
	Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);
	Vector3 Rotate(const Quaternion& q, const Vector3& v);
}
