#pragma once
#include "IFMath.h"

namespace IFE
{
	class DLight
	{
	public:
		struct ConstDLightData
		{
			Vector3 lightv;
			float pad;
			Float3 lightColor;
			uint32_t active;
		};

	private:
		Vector3 lightDir_ = { 1,0,0 };
		Float3 lightColor_ = { 1,1,1 };
		bool active_ = false;

	public:
		inline void SetLightDir(const Vector3& lightDir)
		{
			lightDir_ = lightDir;
			lightDir_ = Vector3Normalize(lightDir);
		}
		inline const Vector3& GetLightDir() { return lightDir_; }
		inline void SetLightColor(const Float3& lightColor) { lightColor_ = lightColor; }
		inline const Float3& GetLightColor() { return lightColor_; }
		inline void SetActive(bool active) { active_ = active; }
		inline bool IsActive() { return active_; }
	};
}