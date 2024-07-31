#pragma once
#include "Component.h"
#include "EditorMode.h"
#include "Animator.h"

namespace IFE
{
	class Attach3DModel : public Component
	{
		std::string boneName_;
		Animator::AnimMat* parentBone_;
		std::string parentName_;

	private:
		void SetTransform();
		void SetTransform(Object3D* obj);
	public:
		void Initialize();
		void Update();
		void Set(Object3D* obj, std::string bone);
		void SetParent(Object3D* obj);
		void SetBorn(std::string bone);

		void LoadingComponent(nlohmann::json& json);
#ifdef EditorMode
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json);
#endif
	};
}
