#pragma once
#include "Camera.h"
#include <list>
#include <memory>
#include "EditorMode.h"

namespace IFE
{
	class CameraManager
	{
		CameraManager() {}
		CameraManager(const CameraManager&) {}
		CameraManager& operator=(const CameraManager&) {}
		~CameraManager();

		std::list<std::unique_ptr<Camera>>cameraList_;

	public:
		static Camera* sActivCamera_;
		inline static Camera* sNextCamera_ = nullptr;
		static CameraManager* Instance();
		void Initialize();
		void LoadingScene();
		void Update();
		void Draw();
		void Reset();
		Camera* Add(const std::string& cameraName);
		Camera* AddInitialize(const std::string& cameraName);
		bool SearchName(const std::string& name);
		Camera* GetCamera(const std::string& name);
		void SetActiveCamera(const std::string& name);

#ifdef InverseEditorMode
#else
		inline static Camera* sDebugCamera_ = nullptr;
		void DebugInitialize();
		void DebugUpdate();
		void DebugGUI();
		void OutputScene();
#endif
	};
}
