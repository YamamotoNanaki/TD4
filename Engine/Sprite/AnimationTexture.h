#pragma once
#include "Component.h"

namespace IFE
{
	class AnimationTexture : public Component
	{
	private:
		uint16_t oldSheetsNumX, oldSheetsNumY;
		uint16_t oldNowNum;
		bool oldFlipX, oldFlipY;
	public:
		uint16_t sheetsNumX_, sheetsNumY_;
		uint16_t nowNum_;
		bool flipX_, flipY_;

	public:
		void Update();
		~AnimationTexture();

#ifdef InverseEditorMode
#else
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};
}
