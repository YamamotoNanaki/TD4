#pragma once
#include "IFMath.h"

namespace IFE
{
	class CShadow
	{
	public:
		struct ConstCShadowData
		{
			Vector3 shadowv;
			float pad;
			Float3 casterPos;
			float distanceCasterLight;
			Float3 shadowAtten;
			float pad3;
			Float2 shadowFactorAngleCos;
			uint32_t active = false;
			float pad4;
		};

	private:
		Vector3 shadowDir_ = { 1,0,0 };
		float distanceCasterLight_ = 100.0f;
		Float3 casterPos_ = { 0,0,0 };
		Float3 shadowAtten_ = { 0.5f,0.6f,0.0f };
		Float2 shadowFactorAngleCos_ = { 0.2f,0.5f };
		bool active_ = false;

	public:
		inline void SetShadowDir(const Vector3& shadowDir)
		{
			shadowDir_ = shadowDir;
			shadowDir_ = Vector3Normalize(shadowDir);
		}
		inline const Vector3& GetShadowDir() { return shadowDir_; }
		inline void SetCasterPos(const Float3& casterPos) { casterPos_ = casterPos; }
		inline const Float3& GetCasterPos() { return casterPos_; }
		inline void SetDistanceCasterLight(float distanceCasterLight) { distanceCasterLight_ = distanceCasterLight; }
		inline const float GetDistanceCasterLight() { return distanceCasterLight_; }
		inline void SetShadowAtten(const Float3& shadowAtten) { shadowAtten_ = shadowAtten; }
		inline const Float3& GetShadowAtten() { return shadowAtten_; }
		inline void SetShadowFactorAngle(const Float2& shadowAngle)
		{
			shadowFactorAngleCos_.x = cosf(ConvertToRadians(shadowAngle.x));
			shadowFactorAngleCos_.y = cosf(ConvertToRadians(shadowAngle.y));
		}
		inline const Float2& GetShadowFactorAngle() { return shadowFactorAngleCos_; }
		inline void SetActive(bool active) { active_ = active; }
		inline bool IsActive() { return active_; }
	};
}