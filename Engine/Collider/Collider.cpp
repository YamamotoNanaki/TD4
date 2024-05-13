#include "Collider.h"
#include "CollideManager.h"
#include "Transform.h"
#include "Object3D.h"
#include "FBXModel.h"
#include "JsonManager.h"

using namespace IFE;

void IFE::ColliderComponent::Initialize()
{
	if (objectPtr_)
	{
		transform_ = objectPtr_->transform_;
		parentPosition_ = &transform_->position_;
		parentScale_ = &transform_->scale_;
	}
	if (colliderType_ == ColliderType::MESH)
	{
		if (meshCollider_)meshCollider_.release();
		auto mesh = new MeshCollider(dynamic_cast<FBXModel*>(objectPtr_->GetModel()), &transform_->matWorld_);
		meshCollider_ = std::unique_ptr<MeshCollider>(mesh);
	}
}

void IFE::ColliderComponent::Draw()
{
}

void IFE::ColliderComponent::Update()
{
	CollideManager::Instance()->ColliderSet(this);
}

Float3 IFE::ColliderComponent::GetOffsetPosition()
{
	return offsetPosition_;
}

Float3 IFE::ColliderComponent::GetOffsetScale()
{
	return offsetScale_;
}

Float3 IFE::ColliderComponent::GetColliderPosition()
{
	if (!parentPosition_)
	{
		GetParentParms();
	}
	return *parentPosition_ + offsetPosition_;
}

Float3 IFE::ColliderComponent::GetColliderScale()
{
	if (!parentScale_)
	{
		GetParentParms();
	}
	return *parentScale_ + offsetScale_;
}

ColliderType IFE::ColliderComponent::GetColliderType()
{
	return colliderType_;
}

void IFE::ColliderComponent::SetColliderType(const ColliderType& colliderType)
{
	colliderType_ = colliderType;
}

void IFE::ColliderComponent::SetOffsetPosition(const Float3& pos)
{
	offsetPosition_ = pos;
}

void IFE::ColliderComponent::SetOffsetScale(const Float3& sca)
{
	offsetScale_ = sca;
}

MeshCollider* IFE::ColliderComponent::GetMeshCollider()
{
	return meshCollider_.get();
}

bool IFE::ColliderComponent::GetPushBackFlag()
{
	return pushBack_;
}

bool IFE::ColliderComponent::GetNoPushBackFlag()
{
	return notPushBack_;
}

bool IFE::ColliderComponent::GetGroundJudgeFlag()
{
	return groundJudge_;
}

void IFE::ColliderComponent::SetPushBackFlag(bool flag)
{
	pushBack_ = flag;
}

void IFE::ColliderComponent::SetNoPushBackFlag(bool flag)
{
	notPushBack_ = flag;
}

void IFE::ColliderComponent::SetGroundJudgeFlag(bool flag)
{
	groundJudge_ = flag;
}

void IFE::ColliderComponent::GetParentParms()
{
	if (objectPtr_)
	{
		parentPosition_ = &objectPtr_->transform_->position_;
		parentScale_ = &objectPtr_->transform_->scale_;
	}
	if (emitterPtr_)
	{
		parentPosition_ = &transformParticle_->position_;
		parentScale_ = &transformParticle_->scale_;
	}
}

void IFE::ColliderComponent::Loading(nlohmann::json& json)
{
	colliderType_ = json["colliderType"];
	pushBack_ = json["pushBack"];
	attribute_ = json["attribute"];
	groundJudge_ = json["groundJudge"];
	notPushBack_ = json["notPushBack"];
}

#ifdef InverseEditorMode
#else
void IFE::ColliderComponent::Output(nlohmann::json& json)
{
	json["colliderType"] = colliderType_;
	json["pushBack"] = pushBack_;
	json["attribute"] = attribute_;
	json["groundJudge"] = groundJudge_;
	json["notPushBack"] = notPushBack_;
}

#include "ImguiManager.h"
void IFE::ColliderComponent::ColliderGUI(uint32_t num)
{
	ImguiManager* gui = ImguiManager::Instance();
	std::string name = "collider" + std::to_string(num);
	if (gui->NewTreeNode(name))
	{
		gui->CheckBoxGUI(&pushBack_, "pushBack");
		gui->CheckBoxGUI(&notPushBack_, "notPushBack");
		gui->CheckBoxGUI(&groundJudge_, "ground judgement");
		if (gui->NewTreeNode("Collider Type"))
		{
			int32_t n = (int32_t)colliderType_;
			gui->RadioButtonGUI("Sphere", &n, (int32_t)ColliderType::SPHERE);
			gui->RadioButtonGUI("Mesh", &n, (int32_t)ColliderType::MESH);
			gui->RadioButtonGUI("Ray", &n, (int32_t)ColliderType::RAY);
			gui->RadioButtonGUI("OBB", &n, (int32_t)ColliderType::OBB);
			colliderType_ = (ColliderType)n;
			gui->EndTreeNode();
		}
		if (gui->NewTreeNode("attribute"))
		{
			int32_t n = (int32_t)attribute_;
			gui->RadioButtonGUI("Landshape", &n, (int32_t)Attribute::LANDSHAPE);
			gui->RadioButtonGUI("Allies", &n, (int32_t)Attribute::ALLIES);
			gui->RadioButtonGUI("Enemys", &n, (int32_t)Attribute::ENEMYS);
			attribute_ = (uint16_t)n;
			gui->EndTreeNode();
		}
		if (gui->ButtonGUI("Delete"))
		{
			componentDeleteFlag_ = true;
		}
		gui->EndTreeNode();
	}
}
#endif

void IFE::Collider::Initialize()
{
	for (auto& itr : colliderList_)
	{
		itr->objectPtr_ = objectPtr_;
		itr->emitterPtr_ = emitterPtr_;
		itr->Initialize();
	}
}

void IFE::Collider::Update()
{
	colliderList_.remove_if([](std::unique_ptr<ColliderComponent>& itr) {return itr->componentDeleteFlag_; });
	for (auto& itr : colliderList_)
	{
		itr->Update();
	}
}

ColliderComponent* IFE::Collider::AddCollider()
{
	std::unique_ptr<ColliderComponent> temp = std::make_unique<ColliderComponent>();
	temp->objectPtr_ = objectPtr_;
	temp->emitterPtr_ = emitterPtr_;
	temp->Initialize();
	colliderList_.push_back(std::move(temp));
	return colliderList_.back().get();
}

void IFE::Collider::LoadingComponent(nlohmann::json& json)
{
	for (auto& j : json["content"])
	{
		auto ptr = AddCollider();
		ptr->Loading(j);
	}
}

#ifdef InverseEditorMode
#else

void IFE::Collider::OutputComponent(nlohmann::json& json)
{
	uint32_t i = 0;
	for (auto& itr : colliderList_)
	{
		itr->Output(json["content"][i]);
		i++;
	}
}

void IFE::Collider::ComponentDebugGUI()
{
	uint32_t i = 0;
	ImguiManager* gui = ImguiManager::Instance();
	if (gui->ButtonGUI(U8("’Ç‰Á")))
	{
		AddCollider();
	}
	for (auto& itr : colliderList_)
	{
		itr->ColliderGUI(i);
		i++;
	}
	colliderList_.remove_if([](std::unique_ptr<ColliderComponent>& itr) {return itr->componentDeleteFlag_; });
}
#endif