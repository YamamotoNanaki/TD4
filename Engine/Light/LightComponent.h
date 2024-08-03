#pragma once
#include "Component.h"
#include "LightManager.h"

namespace IFE
{
	class LightComponent : public Component
	{
	public:
		enum class LightFlag
		{
			Directional, Point, Spot, Shadow
		};

		struct LightSetting
		{
			bool active = true;
			Float3 atten = { 1,1,1 };
			Float3 color = { 1,1,1 };
			Float3 dir = { 1,0,0 };
			Float2 factorAngle = { 0.5f,0.2f };
			float distance = 100;
		};
	private:
		LightFlag lightFlag_ = LightFlag::Point;
		uint8_t lightNum_ = uint8_t(-1);
		inline static LightManager* lm_ = nullptr;

		LightSetting ls_;
	public:
		void Initialize();
		void Update();

		void LoadingComponent(nlohmann::json& json)override;
		inline uint8_t GetLightNum() { return lightNum_; }
		inline LightFlag GetLightFlag() { return lightFlag_; }
		inline LightSetting GetLightSetting() { return ls_; }
		inline void SetLightNum(uint8_t lightNum) { lightNum_ = lightNum; }
		inline void SetLightFlag(LightFlag flag) { lightFlag_ = flag; }
		inline void SetLightSetting(LightSetting setting) { ls_ = setting; }
#ifdef EditorMode
		void DebugInitialize()override;
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
	};
}
