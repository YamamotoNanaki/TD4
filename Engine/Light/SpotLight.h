#pragma once
#include "IFMath.h"

namespace IFE
{
	class SLight
	{
	public:
		struct ConstSLightData
		{
			Vector3 lightv;
			float pad;
			Float3 lightPos;
			float pad1;
			Float3 lightColor;
			float pad2;
			Float3 lightAtten;
			float pad3;
			Float2 lightFactorAngleCos;
			uint32_t active = false;
			float pad4;
		};

	private:
		Vector3 lightDir_ = { 1,0,0 };
		Float3 lightPos_ = { 0,0,0 };
		Float3 lightColor_ = { 1,1,1 };
		Float3 lightAtten_ = { 1.0f,1.0f,1.0f };
		Float2 lightFactorAngleCos_ = { 0.5f,0.2f };
		bool active_ = false;

	public:
		inline void SetLightDir(const Vector3& lightDir)
		{
			lightDir_ = Vector3Normalize(lightDir);
		}
		inline const Vector3& GetLightDir() { return lightDir_; }
		inline void SetLightPos(const Float3& lightPos) { lightPos_ = lightPos; }
		inline const Float3& GetLightPos() { return lightPos_; }
		inline void SetLightColor(const Float3& lightColor) { lightColor_ = lightColor; }
		inline const Float3& GetLightColor() { return lightColor_; }
		inline void SetLightAtten(const Float3& lightAtten) { lightAtten_ = lightAtten; }
		inline const Float3& GetLightAtten() { return lightAtten_; }
		inline void SetLightFactorAngle(const Float2& lightFactorAngle)
		{
			lightFactorAngleCos_.x = cosf(ConvertToRadians(lightFactorAngle.x));
			lightFactorAngleCos_.y = cosf(ConvertToRadians(lightFactorAngle.y));
		}
		inline const Float2& GetLightFactorAngle() { return lightFactorAngleCos_; }
		inline void SetActive(bool active) { active_ = active; }
		inline bool IsActive() { return active_; }
	};
}