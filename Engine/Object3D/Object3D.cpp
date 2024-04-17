#include "Object3D.h"
#include "ObjectManager.h"
#include "PrimitiveModel.h"
#include "GraphicsPipelineManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Texture.h"
#include "TransferGeometryBuffer.h"
#include "Transform.h"
#include "FBXModel.h"
#include "Collider.h"
#include "ImguiManager.h"

using namespace IFE;
using namespace std;

IFE::Object3D::Object3D()
{
	SetObjectPtr(this);
}

void IFE::Object3D::OBJInitialize()
{
	AddComponent<Material>();
	AddComponent<TransferGeometryBuffer>();
	AddComponent<Transform>();
	ComponentManager::Initialize();
	Material* m = GetComponent<Material>();
	m->SetDefaultParameter();
	gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dNormal");
	transform_ = GetComponent<Transform>();
}

void IFE::Object3D::Initialize()
{
	gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dNormal");
	ComponentManager::Initialize();
	transform_ = GetComponent<Transform>();
}

void IFE::Object3D::SetModel(Component* model)
{
	model_ = model;
}

Component* IFE::Object3D::GetModel()
{
	return model_;
}

void IFE::Object3D::SetComponent(std::unique_ptr<Component> component)
{
	AddComponentBack<Component>(std::move(component));
}
void IFE::Object3D::SetComponentFront(std::unique_ptr<Component> component)
{
	AddComponent<Component>(std::move(component));
}

void IFE::Object3D::OBJUpdate()
{
	if (!isActive_)return;
	ComponentManager::Update();
	childCount_ = (int32_t)child_.size();
	if (deleteFlag_ == true)
	{
		for (int32_t i = 0; i < child_.size(); i++)
		{
			child_[i]->deleteFlag_ = true;
		}
	}
}

void IFE::Object3D::DrawBefore(const D3D_PRIMITIVE_TOPOLOGY& topology)
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	commandList->IASetPrimitiveTopology(topology);
}

void IFE::Object3D::Draw()
{
	if (!isActive_)return;
	if (!DrawFlag_)return;
	ComponentManager::Draw();
	model_->Draw();
}

IFE::Object3D::~Object3D()
{
	ComponentManager::Finalize();
}

std::string IFE::Object3D::GetObjectName()
{
	return objectName_;
}

void IFE::Object3D::SetObjectName(const std::string& n)
{
	objectName_ = n;
}

vector<string> IFE::Object3D::GetAllComponentName()
{
	return ComponentManager::GetAllComponentName();
}

void IFE::Object3D::SetActive(bool value)
{
	isActive_ = value;
}

Object3D* IFE::Object3D::GetChild(int32_t index)
{
	return child_[index];
}

void IFE::Object3D::DeleteParent()
{
	for (int32_t i = 0; i < parent_->childName_.size(); i++)
	{
		if (parent_->childName_[i] == objectName_)
		{
			parent_->childName_.erase(childName_.begin() + i);
			parent_->child_.erase(child_.begin() + i);
			break;
		}
	}
	parent_ = nullptr;
	parentName_ = "";
}

void IFE::Object3D::DeleteChild(int32_t index)
{
	Object3D* obj = GetChild(index);
	obj->parent_ = nullptr;
	obj->parentName_ = "";
	childName_.erase(childName_.begin() + index);
	child_.erase(child_.begin() + index);
}

void IFE::Object3D::DeleteChild(const std::string& c)
{
	for (int32_t i = 0; i < childName_.size(); i++)
	{
		if (childName_[i] == c)
		{
			DeleteChild(i);
			return;
		}
	}
}

void IFE::Object3D::DeleteChildAll()
{
	for (int32_t i = 0; i < parent_->child_.size(); i++)
	{
		child_[i]->parent_ = nullptr;
		child_[i]->parentName_ = "";
	}
	child_.clear();
}

void IFE::Object3D::CopyValue(Object3D* ptr)
{
	ptr->isActive_ = isActive_;
	ptr->deleteFlag_ = deleteFlag_;
	ptr->DrawFlag_ = DrawFlag_;
	ptr->model_ = model_;
	ptr->gp_ = gp_;
	ComponentManager::CopyValue(ptr);
	ptr->transform_ = ptr->GetComponent<Transform>();
	ptr->SetTransform();
}

void IFE::Object3D::Destroy()
{
	deleteFlag_ = true;
}

bool IFE::Object3D::GetDeleteFlag()
{
	return deleteFlag_;
}

#ifdef NDEBUG
#else
void IFE::Object3D::DebugGUI(bool fdelete, bool fmove, std::string* str)
{
	bool m = false;
	if (ImguiManager::Instance()->ObjectGUI(objectName_, fdelete, fmove, &m, this))
	{
		deleteFlag_ = true;
	}
	if (m)
	{
		*str = objectName_;
	}
}

void IFE::Object3D::ComponentGUI()
{
	std::function<void(std::unique_ptr<Component>)> addFunc = [&](std::unique_ptr<Component> com)
		{
			SetComponent(std::move(com));
		};
	std::function<void(void)> es = [&]()
		{
			ImguiManager::Instance()->CheckBoxGUI(&DrawFlag_, "Draw Flag");
		};
	std::function<void(void)>f = [&]()
		{
			ImguiManager::Instance()->CollapsingHeaderGUI("Object Setting", es);
			ComponentManager::DebugGUI();
		};
	std::function<void(Component*)>modelFunc = [&](Component* com)
		{
			model_ = com;
		};
	ImguiManager::Instance()->ComponentGUI(objectName_, f, addFunc, modelFunc);
}

void IFE::Object3D::OutputScene(nlohmann::json& j, bool flag)
{
	string s = objectName_;
	if (flag)
	{
		s = "P" + objectName_;
	}
	j["name"] = s;
	j["drawFlag"] = DrawFlag_;
	j["model"] = model_->componentName_;
	//j["backGround"] = backGround_;
	uint32_t i = 0;
	for (auto& com : componentList_)
	{
		j["component"][i] = com->componentName_;
		i++;
	}
	ComponentManager::OutputScene(j);
}
void IFE::Object3D::DebugUpdate()
{
	ComponentManager::DebugUpdate();
	childCount_ = (int32_t)child_.size();
	if (deleteFlag_ == true)
	{
		for (int32_t i = 0; i < child_.size(); i++)
		{
			child_[i]->deleteFlag_ = true;
		}
	}
}

#endif
void IFE::Object3D::LoadingScene(nlohmann::json& j)
{
	DrawFlag_ = j["drawFlag"];
	for (auto& com : j["component"])
	{
		ComponentManager::LoadingScene(j, com);
	}
}

void IFE::Object3D::LoadChild()
{
	if (parentName_ != "")
	{
		parent_ = ObjectManager::Instance()->GetObjectPtr(parentName_);
	}
	for (int32_t i = 0; i < childName_.size(); i++)
	{
		child_.push_back(ObjectManager::Instance()->GetObjectPtr(childName_[i]));
	}
}
