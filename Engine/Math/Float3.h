#pragma once

namespace IFE
{
	struct Float3
	{
		float x;
		float y;
		float z;

		Float3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		void Set(float x, float y, float z);
		Float3 operator+()const;
		Float3 operator-()const;
		Float3 operator*()const;
		Float3& operator+=(float f);
		Float3& operator-=(float f);
		Float3& operator+=(const Float3& f);
		Float3& operator-=(const Float3& f);
		Float3& operator*=(const Float3& f);
		Float3& operator/=(const Float3& f);
		Float3& operator*=(float f);
		Float3& operator/=(float f);
		Float3 operator=(float* f);
		Float3 operator=(const Float3& f);
		Float3 operator=(float f);

		bool operator==(const Float3& f);
		bool operator!=(const Float3& f);

		float& operator[](size_t index);
		const float& operator[](size_t index)const;

		static Float3 zero() { return Float3(0, 0, 0); };
		//virtual ~Float3() {};
	};
	Float3 operator/(const Float3& f, float s);
	Float3 operator*(const Float3& f, float s);
	Float3 operator*(float s, const Float3& f);
	Float3 operator+(const Float3& f1, const Float3& f2);
	Float3 operator-(const Float3& f1, const Float3& f2);
};