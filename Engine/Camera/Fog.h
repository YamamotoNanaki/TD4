#pragma once
#include "Component.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"

namespace IFE
{
	class Fog : public Component
	{
		using Component::Component;
	private:
		std::unique_ptr<ConstBuffer<ConstBufferFog>> fogBuffer_;
		ConstBufferFog* constMapFog_ = nullptr;
	public:
		Float4 color_ = { 1,1,1,1 };
		float fogNear_ = 650.0f; //フォグの開始位置
		float fogFar_ = 700.0f; //フォグの終了位置
		bool fogFlag_ = false; //フォグの終了位置

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void Copy(Component* component)override;
#ifdef InverseEditorMode
#else
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void DebugInitialize()override;
		void LoadingComponent(nlohmann::json& json)override;
	public:
	};
}
