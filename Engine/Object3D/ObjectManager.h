#pragma once
#include "Object3D.h"
#include "IFEMath.h"
#include <list>
#include <memory>
#include "EditorMode.h"

namespace IFE
{
	class ObjectManager
	{
		std::list<std::unique_ptr<Object3D>>objectList_;
		std::list<std::unique_ptr<Object3D>>prefabList_;


		ObjectManager() {}
		ObjectManager(const ObjectManager&) {}
		ObjectManager& operator=(const ObjectManager&) {}
		~ObjectManager() {}
	public:
		static ObjectManager* Instance();
		static void Finalize();
		void OBJInitialize();
		void Initialize();
		void Update();
		void Draw();
		//void DrawBackGround();
		Object3D* AddInitialize(const std::string& objectName, Component* model);
		Object3D* Add(const std::string& objectName);
		Object3D* Add(const std::string& objectName, Component* model);
		Object3D* AddPrefab(const std::string& objectName);
		Object3D* GetObjectPtr(const std::string& objectName);
		Object3D* GetPrefab(const std::string& name);
		void SetModel(const std::string& objectName, Component* model);
		std::list<std::unique_ptr<Object3D>>& GetObjList();
		void Reset();
		bool SearchName(const std::string& name);
		Object3D* SearchObject(const std::string& name);
		Object3D* Instantiate(const std::string& objectName, const std::string& newObjectName = "");
		Object3D* Instantiate(const std::string& objectName, const Float3& position, const std::string& newObjectName = "");
		void ObjectSwap(int32_t num1, int32_t num2);
		void ObjectMoveElement(int32_t num1, int32_t num2);

		/// <summary>
		/// 共通するオブジェクトの名前があった場合番号を自動的につける関数
		/// </summary>
		/// <returns></returns>
		std::string SetNameNumber(std::string objectName);

#ifdef InverseEditorMode
#else
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
		std::string GetObjectNameGUI();
#endif
		void LoadingScene();
	};
}