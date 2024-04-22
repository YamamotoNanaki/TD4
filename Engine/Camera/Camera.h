#pragma once
#include "View.h"
#include "Projection.h"
#include "Quaternion.h"
#include "IFEMath.h"
#include "ComponentManager.h"
#include "EditorMode.h"

#pragma warning(push)
#pragma warning(disable:4244)
#include <memory>
#pragma warning(pop)

namespace IFE
{
	class TransformCamera;
	class Camera :public ComponentManager
	{
		View view_;
		Projection projection_;
		Quaternion rotation_;
		Float3 position_;
		Float3 eulerAngleDegrees_;

	public:
		TransformCamera* transform_;
		std::string cameraName_;
		bool deleteFlag_ = false;

	public:
		Camera();
		void Initialize();
		void CameraInitialize();
		void CameraUpdate();
		View* GetView();
		Projection* GetProjection();
		~Camera();
		void SetComponent(std::unique_ptr<Component> component);
		void SetComponentFront(std::unique_ptr<Component> component);

#ifdef InverseEditorMode
#else
		void DebugUpdate();
		void DebugCameraInitialize();
		void DebugGUI(bool fdelete);
		void ComponentGUI();
		void OutputScene(nlohmann::json& json);
#endif
		void LoadingScene(nlohmann::json& json);
	};
}
