#pragma once

namespace IFE
{
	struct Float4
	{
		float x;
		float y;
		float z;
		float w;

		Float4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
		void Set(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
		Float4 operator+()const;
		Float4 operator-()const;
		Float4 operator*()const;
		Float4& operator+=(const Float4& f);
		Float4& operator-=(const Float4& f);
		Float4& operator*=(const Float4& f);
		Float4& operator/=(const Float4& f);
		Float4& operator*=(float f);
		Float4& operator/=(float f);

		Float4 operator=(const Float4& f);
		Float4 operator=(float* f);
		Float4 operator=(float f);

		float& operator[](size_t index);
		const float& operator[](size_t index)const;
	};
};