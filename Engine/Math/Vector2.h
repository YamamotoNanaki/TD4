#pragma once
#include "Float2.h"

namespace IFE
{
	struct Vector2 :public Float2
	{
		Vector2();
		Vector2(float x, float y);
		void Set(float x, float y);

		float Length() const;
		Vector2& Normalize();
		float Dot(const Vector2& v) const;
		float Cross(const Vector2& v) const;

		Vector2 operator+() const;
		Vector2 operator-() const;
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
		Vector2& operator= (const Vector2& obj) = default;
		Vector2& operator= (const Float2& obj);
		static Vector2 zero;
		//bool operator==(const Vector2& v);
	};
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator/(const Vector2& v1, float s);
};
