#pragma once
#include <d3dx12.h>
#include <memory>
#include "IFEMath.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "ConstBuffer.h"

namespace IFE
{
	class LightManager
	{
	public:
		static const uint32_t s_LIGHT_MAX_ = 200;
		static const uint32_t s_PLIGHT_NUM = s_LIGHT_MAX_;
		static const uint32_t s_DLIGHT_NUM = 3;
		static const uint32_t s_SLIGHT_NUM = 3;
		static const uint32_t s_CSHADOW_NUM = 3;

	public:
		struct ConstBufferData
		{
			Float3 ambientColor;
			float pad1;
			DLight::ConstDLightData dLights[s_DLIGHT_NUM];
			PLight::ConstPLightData pLights[s_PLIGHT_NUM];
			SLight::ConstSLightData sLights[s_SLIGHT_NUM];
			CShadow::ConstCShadowData cShadows[s_CSHADOW_NUM];
		};
	private:
		inline static uint8_t sNextPNum_ = 0;

	private:
		std::unique_ptr<ConstBuffer<ConstBufferData>>constBuff_;
		Float3 ambientColor_ = { 1,1,1 };
		DLight dLight_[s_DLIGHT_NUM];
		PLight pLight_[s_PLIGHT_NUM];
		SLight sLight_[s_SLIGHT_NUM];
		CShadow cShadow_[s_CSHADOW_NUM];
		bool dirty_ = false;
		LightManager() {}
		LightManager(const LightManager&) {}
		LightManager& operator=(const LightManager&) {}
		~LightManager() {}

	public:
		void Initialize();
		void Reset();
		void TransferConstBuffer();
		void SetConstBufferPointLight(uint8_t index);
		void DefaultLightSetting();
		void SetAmbientColor(const Float3& color);
		void SetDirLightActive(int32_t index, bool active);
		void SetDirLightDir(int32_t index, const Vector3& lightdir);
		void SetDirLightColor(int32_t index, const Float3& lightcolor);
		void SetPointLightActive(int32_t index, bool active);
		bool GetPointLightIsActive(int32_t index);
		void SetPointLightPos(int32_t index, const Float3& lightpos);
		void SetPointLightColor(int32_t index, const Float3& lightcolor);
		void SetPointLightAtten(int32_t index, const Float3& lightAtten);
		void SetSpotLightActive(int32_t index, bool active);
		void SetSpotLightDir(int32_t index, const Vector3& lightdir);
		void SetSpotLightPos(int32_t index, const Float3& lightpos);
		void SetSpotLightColor(int32_t index, const Float3& lightcolor);
		void SetSpotLightAtten(int32_t index, const Float3& lightAtten);
		void SetSpotLightFactorAngle(int32_t index, const Float2& lightFactorAngle);
		void SetCircleShadowActive(int32_t index, bool active);
		void SetCircleShadowDir(int32_t index, const Vector3& shadowdir);
		void SetCircleShadowCasterPos(int32_t index, const Float3& shadowpos);
		void SetCircleShadowDistanceCasterLight(int32_t index, float distanceCasterLight);
		void SetCircleShadowAtten(int32_t index, const Float3& shadowAtten);
		void SetCircleShadowFactorAngle(int32_t index, const Float2& shadowFactorAngle);
		void Update();
		static uint8_t GetPointLightNumber();
		void Draw(uint32_t rootParameterIndex);
		static LightManager* Instance();
		static void Finalize();

#ifdef InverseEditorMode
#else
		void DebugGUI();
		void OutputScene();
#endif
		void LoadingScene();
	};
}