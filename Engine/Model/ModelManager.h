#pragma once
#include "Component.h"
#include "ModelLoader.h"
#include "AddModelSettings.h"
#include <list>
#include <memory>
#include <string>
#include "EditorMode.h"

namespace IFE
{
	class ModelManager
	{
		std::list<std::unique_ptr<Component>>modelList_;
		ModelLoader modelLoader_;

		ModelManager() {}
		ModelManager(const ModelManager&) {}
		ModelManager& operator=(const ModelManager&) {}
		~ModelManager() {}

	public:
		static ModelManager* Instance();
		static void Finalize();
		void Update();
		void Draw();
		void Add(const std::string& modelName, const AddModelSettings& modelSetting, const std::string& fileName = "", bool smooth = false);
		Component* GetModel(const std::string& modelName);
		void Reset();
		std::vector<std::string>GetAllModelName();
#ifdef InverseEditorMode
#else
		void DebugGUI();
		std::string GetModelNameGUI();
		void OutputScene();
#endif
		void LoadingScene();
	};
}
