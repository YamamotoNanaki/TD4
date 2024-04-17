#pragma once
#include "Component.h"

namespace IFE
{
	class RectTexture : public Component
	{
		Float2 texSize_;
		Float2 texBase_;
		Float2 initSize_;
		bool init = true;
	public:
		void Initialize();
		void Update();
		~RectTexture();

#ifdef NDEBUG
#else
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};
}
