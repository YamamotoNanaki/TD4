#include "JsonManager.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "Debug.h"
#include "ImguiManager.h"
#include "TransferGeometryBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "Rigidbody.h"
#include "ComponentHelp.h"
#include "CollideManager.h"
#include "LightManager.h"
#include "CameraManager.h"
#include <regex>

using namespace IFE;
using namespace std;

ObjectManager* IFE::ObjectManager::Instance()
{
	static ObjectManager inst;
	return &inst;
}

void IFE::ObjectManager::OBJInitialize()
{
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		itr->OBJInitialize();
	}
	for (unique_ptr<Object3D>& itr : prefabList_)
	{
		itr->OBJInitialize();
	}
}

void IFE::ObjectManager::Initialize()
{
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		itr->Initialize();
	}
}

void IFE::ObjectManager::Update()
{
	objectList_.remove_if([](unique_ptr<Object3D>& obj) {return obj->GetDeleteFlag(); });
	prefabList_.remove_if([](unique_ptr<Object3D>& obj) {return obj->GetDeleteFlag(); });
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		itr->OBJUpdate();
	}
}

void IFE::ObjectManager::Draw()
{
	Object3D::DrawBefore();

	list<Object3D*>objList;
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		if (!itr->isActive_)continue;
		if (!itr->DrawFlag_)continue;
		//if (itr->backGround_)continue;
		objList.push_back(itr.get());
	}

	objList.sort([](const Object3D* objA, const Object3D* objB) {return objA->gp_->pipelineNum_ > objB->gp_->pipelineNum_; });
	uint8_t num = 255;
	for (auto& itr : objList)
	{
		if (num != itr->gp_->pipelineNum_)
		{
			itr->gp_->SetDrawBlendMode();
			num = itr->gp_->pipelineNum_;
			LightManager::Instance()->Draw(3);
			CameraManager::Instance()->Draw();
		}
		itr->Draw();
	}
}

//void IFE::ObjectManager::DrawBackGround()
//{
//	Object3D::DrawBefore();
//
//	list<Object3D*>objList;
//	for (unique_ptr<Object3D>& itr : objectList_)
//	{
//		if (!itr->isActive_)continue;
//		if (!itr->DrawFlag_)continue;
//		//if (!itr->backGround_)continue;
//		objList.push_back(itr.get());
//	}
//
//	objList.sort([](const Object3D* objA, const Object3D* objB) {return objA->gp_->pipelineNum_ > objB->gp_->pipelineNum_; });
//	uint8_t num = 255;
//	for (auto& itr : objList)
//	{
//		if (num != itr->gp_->pipelineNum_)
//		{
//			itr->gp_->SetDrawBlendMode();
//			num = itr->gp_->pipelineNum_;
//			LightManager::Instance()->Draw(3);
//		}
//		itr->Draw();
//	}
//}

Object3D* IFE::ObjectManager::AddInitialize(const std::string& objectName, Component* model)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	ptr->OBJInitialize();
	ptr->SetObjectName(SetNameNumber(objectName));
	ptr->SetModel(model);
	objectList_.push_back(std::move(ptr));
	return objectList_.back().get();
}

Object3D* IFE::ObjectManager::Add(const std::string& objectName)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	ptr->SetObjectName(SetNameNumber(objectName));
	objectList_.push_back(std::move(ptr));
	return objectList_.back().get();
}

Object3D* IFE::ObjectManager::AddPrefab(const std::string& objectName)
{
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	ptr->SetObjectName(SetNameNumber(objectName));
	prefabList_.push_back(std::move(ptr));
	return prefabList_.back().get();
}

Object3D* IFE::ObjectManager::GetObjectPtr(const std::string& objectName)
{
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		if (itr->GetObjectName() == objectName)
		{
			return itr.get();
		}
	}
	for (unique_ptr<Object3D>& itr : prefabList_)
	{
		if (itr->GetObjectName() == objectName)
		{
			return itr.get();
		}
	}
	return nullptr;
}

Object3D* IFE::ObjectManager::GetPrefab(const std::string& name)
{
	for (unique_ptr<Object3D>& itr : prefabList_)
	{
		if (itr->GetObjectName() == name)return itr.get();
	}
	return nullptr;
}

void IFE::ObjectManager::SetModel(const std::string& objectName, Component* model)
{
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		if (itr->GetObjectName() == objectName)
		{
			itr->SetModel(model);
			return;
		}
	}
	for (unique_ptr<Object3D>& itr : prefabList_)
	{
		if (itr->GetObjectName() == objectName)
		{
			itr->SetModel(model);
			return;
		}
	}
}

std::list<std::unique_ptr<Object3D>>* IFE::ObjectManager::GetObjList()
{
	return &objectList_;
}

void IFE::ObjectManager::Reset()
{
	objectList_.clear();
	prefabList_.clear();
}

bool IFE::ObjectManager::SearchName(const std::string& name)
{
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		if (itr->GetObjectName() == name)return true;
	}
	for (unique_ptr<Object3D>& itr : prefabList_)
	{
		if (itr->GetObjectName() == name)return true;
	}
	return false;
}

Object3D* IFE::ObjectManager::SearchObject(const std::string& name)
{
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		if (itr->GetObjectName() == name)return itr.get();
	}
	for (unique_ptr<Object3D>& itr : prefabList_)
	{
		if (itr->GetObjectName() == name)return itr.get();
	}
	return nullptr;
}

Object3D* IFE::ObjectManager::Instantiate(const std::string& objectName, const std::string& newObjectName)
{
	return Instantiate(objectName, { 0,0,0 }, newObjectName);
}

Object3D* IFE::ObjectManager::Instantiate(const std::string& objectName, const Float3& position, const std::string& newObjectName)
{
	auto obj = SearchObject(objectName);
	if (obj == nullptr)return nullptr;
	std::unique_ptr<Object3D> ptr = make_unique<Object3D>();
	string s = newObjectName;
	if (s == "")
	{
		s = objectName + "(add)";
	}
	ptr->SetObjectName(s);
	obj->CopyValue(ptr.get());
	ptr->transform_->position_ = position;
	objectList_.push_back(std::move(ptr));
	return objectList_.back().get();
}

std::string IFE::ObjectManager::SetNameNumber(std::string objectName)
{
	string objectN = std::regex_replace(objectName, regex("\\d"), "");
	string name = objectN;
	uint32_t num = 0;
	while (SearchName(name))
	{
		num++;
		name = objectN + to_string(num);
	}
	return name;
}

#ifdef NDEBUG
#else
#include "imgui.h"
void IFE::ObjectManager::DebugGUI()
{
	static bool add = false;
	static bool fdelete = false;
	static bool prefab = false;
	static bool moveF = false;
	ImguiManager* im = ImguiManager::Instance();
	im->ObjectManagerGUI(&add, &fdelete, &prefab, &moveF);
	if (prefab)
	{
		im->TextGUI("PrefabManager");
	}
	else
	{
		im->TextGUI("ObjectManager");
	}
	if (add)
	{
		std::function<string(void)> func = []()
			{
				return ModelManager::Instance()->GetModelNameGUI();
			};
		static string name;
		static string model;
		static bool pre = false;
		if (pre)
		{
			im->CheckBoxGUI(&pre, "add new object");
			static int32_t num = 0;
			std::function<void(void)>prefunc = [&]()
				{
					static char n[256];
					ImGui::InputText("Add Object Name", n, sizeof(n));
					name = n;
					int32_t i = 0;
					for (unique_ptr<Object3D>& itr : prefabList_)
					{
						ImGui::RadioButton(itr->GetObjectName().c_str(), &num, i);
						i++;
					}
					if (ImGui::Button("Add Object"))
					{
						string s;
						int32_t j = 0;
						for (unique_ptr<Object3D>& itr : prefabList_)
						{
							if (num == j)
							{
								s = itr->GetObjectName();
								break;
							}
							j++;
						}
						if (s != "")
						{

						}
						else
						{
							im->TextGUI("prefab not found");
						}
					}
				};
			if (prefabList_.size() != 0)im->CollapsingHeaderGUI("Add from Prefab", prefunc);
			else
			{
				im->TextGUI("prefab not found");
			}
		}
		else
		{
			im->CheckBoxGUI(&pre, "add from prefab");
			if (im->ObjectAddGUI(&name, &model, func))
			{
				if (name == "")
				{
					im->TextGUI("error : no name has been set");
				}
				else if (SearchName(name))
				{
					im->TextGUI("error : an object with the same name exists");
				}
				else
				{
					AddInitialize(name, ModelManager::Instance()->GetModel(model));
					im->TextGUI("success");
				}
			}
		}
	}
	string str;
	if (!prefab)
	{
		for (unique_ptr<Object3D>& itr : objectList_)
		{
			string s;
			itr->DebugGUI(fdelete, moveF, &s);
			if (s != "")
			{
				str = s;
			}
		}

		im->EndGUI();
		for (unique_ptr<Object3D>& itr : objectList_)
		{
			itr->ComponentGUI();
		}

		for (unique_ptr<Object3D>& itr : objectList_)
		{
			if (itr->GetObjectName() == str)
			{
				prefabList_.push_back(std::move(itr));
				objectList_.remove(itr);
				break;
			}
		}
	}
	else
	{
		for (unique_ptr<Object3D>& itr : prefabList_)
		{
			string s;
			itr->DebugGUI(fdelete, moveF, &s);
			if (s != "")
			{
				str = s;
			}
		}

		im->EndGUI();
		for (unique_ptr<Object3D>& itr : prefabList_)
		{
			itr->ComponentGUI();
		}
		for (unique_ptr<Object3D>& itr : prefabList_)
		{
			if (itr->GetObjectName() == str)
			{
				objectList_.push_back(std::move(itr));
				prefabList_.remove(itr);
				break;
			}
		}
	}

}

void IFE::ObjectManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& j = jm->GetJsonData();
	uint32_t i = 0;
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		itr->OutputScene(j[i]);
		j[i]["prefab"] = false;
		i++;
	}
	for (unique_ptr<Object3D>& itr : prefabList_)
	{
		itr->OutputScene(j[i]);
		j[i]["prefab"] = true;
		i++;
	}
	jm->Output("ObjectManager");
}
void IFE::ObjectManager::DebugUpdate()
{
	objectList_.remove_if([](unique_ptr<Object3D>& obj) {return obj->GetDeleteFlag(); });
	prefabList_.remove_if([](unique_ptr<Object3D>& obj) {return obj->GetDeleteFlag(); });
	for (unique_ptr<Object3D>& itr : objectList_)
	{
		itr->DebugUpdate();
	}
}
#endif
void IFE::ObjectManager::LoadingScene()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("ObjectManager");
	nlohmann::json js = jm->GetJsonData();
	for (auto& j : js)
	{
		Object3D* obj;
		if (j["prefab"])obj = AddPrefab(j["name"]);
		else obj = Add(j["name"]);
		obj->SetModel(ModelManager::Instance()->GetModel(j["model"]));
		obj->LoadingScene(j);
		obj->Initialize();
		string s = j["name"];
	}
}
