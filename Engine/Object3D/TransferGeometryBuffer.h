#pragma once
#include "Component.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"
#include <memory>

namespace IFE
{
	class TransferGeometryBuffer : public Component
	{
		using Component::Component;
		std::unique_ptr<ConstBuffer<ConstBufferModelGeometry>> geometryBuffer_;
		ConstBufferModelGeometry* constMapGeometry_ = nullptr;
	public:
		float explosion_ = 0;
		float gravity_ = 0;
		float rotation_ = 0;
		float polygonSize_ = 1;
		bool lightFlag_ = true;
		bool toonFlag_ = false;
		bool bllomFlag_ = false;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		void Copy(Component* component)override;
#ifdef NDEBUG
#else
		void DebugGUI();
		void OutputComponent(nlohmann::json&json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};
}
