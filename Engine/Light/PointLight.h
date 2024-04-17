#pragma once
#include "IFMath.h"

namespace IFE
{
	class PLight
	{
	public:
		struct ConstPLightData
		{
			Float3 lightPos;
			float pad1;
			Float3 lightColor;
			float pad2;
			Float3 lightAtten;
			uint32_t active;
		};

	private:
		Float3 lightPos_ = { 0,0,0 };
		Float3 lightColor_ = { 1,1,1 };
		Float3 lightAtten_ = { 1.0f,1.0f,1.0f };
		bool active_ = false;

	public:
		inline void SetLightPos(const Float3& lightPos) { lightPos_ = lightPos; }
		inline const Float3 GetLightPos() { return lightPos_; }
		inline void SetLightColor(const Float3& lightColor) { lightColor_ = lightColor; }
		inline const Float3 GetLightColor() { return lightColor_; }
		inline void SetLightAtten(const Float3& lightAtten) { lightAtten_ = lightAtten; }
		inline const Float3 GetLightAtten() { return lightAtten_; }
		inline void SetActive(bool active) { active_ = active; }
		inline bool IsActive() { return active_; }
	};
}