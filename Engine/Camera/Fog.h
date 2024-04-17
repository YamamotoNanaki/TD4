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
		float fogNear_ = 650.0f; //�t�H�O�̊J�n�ʒu
		float fogFar_ = 700.0f; //�t�H�O�̏I���ʒu
		bool fogFlag_ = false; //�t�H�O�̏I���ʒu

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void Copy(Component* component)override;
#ifdef NDEBUG
#else
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	public:
	};
}
