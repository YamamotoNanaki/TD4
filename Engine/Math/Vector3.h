#pragma once
#include "Float3.h"

namespace IFE
{
	struct Vector3 : public Float3
	{
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Float3& f3);
		void Set(const Float3& start, const Float3& end);
		void Set(float x, float y, float z);

		float Length() const;
		Vector3& Normalize();
		float Dot(const Vector3& v) const;
		Vector3 Cross(const Vector3& v)const;

		Vector3 operator+() const;
		Vector3 operator-() const;
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);

		static Vector3 up() { return Vector3(0, 1, 0); };
		static Vector3 right() { return Vector3(1, 0, 0); };
		static Vector3 forward() { return Vector3(0, 0, 1); };
	};

	Vector3 VectorSubtract(const Vector3& v1, const Vector3& v2);
	Vector3 operator/(const Vector3& v, float s);
	Vector3 operator*(const Vector3& v, float s);
	Vector3 operator*(float s, const Vector3& v);
	Vector3 operator+(const Vector3& v1, const Vector3& v2);
	Vector3 operator-(const Vector3& v1, const Vector3& v2);
};
