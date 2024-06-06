#pragma once
#include "Texture.h"
#include "ComponentManager.h"
#include "Component.h"
#include "JsonManager.h"
#include "GraphicsPipeline.h"
#include <memory>
#include "EditorMode.h"
#include "IFEMath.h"

namespace IFE
{
	class Transform;
	class Object3D final :public ComponentManager
	{
	private:
		std::string objectName_;
		bool deleteFlag_ = false;
	public:
		bool isActive_ = true;
		bool DrawFlag_ = true;
		//bool backGround_ = false;
		Transform* transform_ = nullptr;
		Object3D* parent_ = nullptr;
		std::vector<Object3D*> child_;
		std::string parentName_;
		std::vector<std::string> childName_;
		int32_t childCount_ = 0;
		Component* model_ = nullptr;
		GraphicsPipeline* gp_ = nullptr;

	public:
		Object3D();
		void OBJInitialize();
		void Initialize();
		void SetModel(Component* model);
		Component* GetModel();
		void SetComponent(std::unique_ptr<Component> component);
		void SetComponentFront(std::unique_ptr<Component> component);
		void OBJUpdate();
		static void DrawBefore(const D3D_PRIMITIVE_TOPOLOGY& topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		void Draw();
		~Object3D();
		std::string GetObjectName();
		void SetObjectName(const std::string& objectName);
		std::vector<std::string> GetAllComponentName();
		void SetActive(bool value);
		Object3D* GetChild(int32_t index);
		void DeleteParent();
		void DeleteChild(int32_t childNum);
		void DeleteChild(const std::string& childName);
		void DeleteChildAll();

		void CopyValue(Object3D* ptr);
		void Destroy();
		bool GetDeleteFlag();
		void SetColor(const Float4& color);
		void SetColor255(const Float4& color);
		Float4 GetColor();
		Float4 GetColor255();

#ifdef InverseEditorMode
#else
		void DebugGUI(bool flagdelete, bool fmove, std::string* str);
		void ComponentGUI();
		void OutputScene(nlohmann::json& j, bool prefab = false);
		void DebugUpdate();
#endif
		void LoadingScene(nlohmann::json& json);
		void LoadChild();
		void DebugInitialize();
	};
}
