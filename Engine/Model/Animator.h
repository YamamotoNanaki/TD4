#pragma once
#include "Component.h"
#include "FBXModel.h"
#include "ConstStruct.h"
#include "ConstBuffer.h"

namespace IFE
{
	class Animator : public Component
	{
		using Component::Component;
		uint8_t oldAnimNum_ = 0;
		uint8_t interpolationAnimNum_ = 0;
		FBXModel* model_;
		float animTimer_ = 0;
		float oldAnimTimer_ = 0;
		float lerpTimer_ = 0;
		bool interpolation_ = false;
		float interpolationMaxTimer_ = -1;
		float oldAnimSpeed_ = 1.f;
		bool oldLoop_ = true;
		std::unique_ptr<ConstBuffer<ConstBufferDataSkin>> skinBuffer_;
		ConstBufferDataSkin* constMapSkin_ = nullptr;
		uint8_t animNum_ = 0;
	public:
		float animSpeed_ = 1.f;
		bool loop_ = true;
		bool animEnd_ = false;
		bool animFlag_ = true;

	public:
		void Initialize()override;
		void DebugInitialize()override;
		void Update()override;
		void Draw()override;
		float GetEndTime();
		~Animator();

		void SetAnimation(std::string animName, bool interpolation = true, float interpolationMaxTimer = 0.2);
		std::string GetAnimation();

		void ModelUpdate();

#ifdef InverseEditorMode
#else
		void ComponentDebugGUI()override;
#endif
	};
}
