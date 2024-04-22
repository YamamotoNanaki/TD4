#pragma once
#include "Component.h"
#include "FBXModel.h"
#include "ConstStruct.h"
#include "ConstBuffer.h"

namespace IFE
{
	class Animator : public Component
	{
		uint8_t oldAnimNum_ = 0;
		FBXModel* model_;
		float animTimer_ = 0;
		std::unique_ptr<ConstBuffer<ConstBufferDataSkin>> skinBuffer_;
		ConstBufferDataSkin* constMapSkin_ = nullptr;
		uint8_t animNum_ = 0;
	public:
		float animSpeed_ = 1.f;
		bool loop_ = false;
		bool animEnd_ = false;

	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		float GetEndTime();
		~Animator();

		void SetAnimation(std::string animName);
		std::string GetAnimation();

#ifdef InverseEditorMode
#else
		void ComponentDebugGUI()override;
#endif
	};
}
