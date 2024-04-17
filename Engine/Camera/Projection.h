#pragma once
#include "IFMath.h"

namespace IFE
{
	class Projection
	{
	private:
		Matrix matProjection_;
	public:
		float nearZ_, farZ_;
		float winWidth_, winHeight_;
		float fovAngle_;

	public:
		Projection(float radian, float winWidth, float winHeight, float nearZ = 0.1f, float farZ = 1000.0f);
		Projection(float radian = 45, float nearZ = 0.1f, float farZ = 1000.0f);
		void Inisialize(float radian, float winWidth, float winHeight, float nearZ = 0.1f, float farZ = 1000.0f);
		void Update();
		Matrix Get()const;
		Matrix* GetAddressOf();
	};
}