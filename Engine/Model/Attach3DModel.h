#pragma once
#include "Component.h"
#include "EditorMode.h"
#include "FBXModel.h"

namespace IFE
{
	class Attach3DModel : public Component
	{
		std::string boneName_;
		Bone* parentBone_;
		std::string parentName_;

	private:
		void SetTransform();
	public:
		void Initialize();
		void Update();

		void LoadingComponent(nlohmann::json& json);
#ifdef EditorMode
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json);
#endif
	};
}
