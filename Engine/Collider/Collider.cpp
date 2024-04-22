#include "Collider.h"
#include "CollideManager.h"
#include "Transform.h"
#include "Object3D.h"
#include "FBXModel.h"
#include "JsonManager.h"

using namespace IFE;

void IFE::Collider::Initialize()
{
	if (colliderType_ == ColliderType::MESH)
	{
		if (meshCollider_)meshCollider_.release();
		auto mesh = new MeshCollider(dynamic_cast<FBXModel*>(objectPtr_->GetModel()), &transform_->matWorld_);
		meshCollider_ = std::unique_ptr<MeshCollider>(mesh);
	}
}

void IFE::Collider::Draw()
{
}

void IFE::Collider::Update()
{
	CollideManager::Instance()->ColliderSet(this);
}

Float3 IFE::Collider::GetOffsetPosition()
{
	return offsetPosition_;
}

Float3 IFE::Collider::GetOffsetScale()
{
	return offsetScale_;
}

Float3 IFE::Collider::GetColliderPosition()
{
	if (!parentPosition_)
	{
		GetParentParms();
	}
	return *parentPosition_ + offsetPosition_;
}

Float3 IFE::Collider::GetColliderScale()
{
	if (!parentScale_)
	{
		GetParentParms();
	}
	return *parentScale_ + offsetScale_;
}

ColliderType IFE::Collider::GetColliderType()
{
	return colliderType_;
}

void IFE::Collider::SetColliderType(const ColliderType& colliderType)
{
	colliderType_ = colliderType;
}

void IFE::Collider::SetOffsetPosition(const Float3& pos)
{
	offsetPosition_ = pos;
}

void IFE::Collider::SetOffsetScale(const Float3& sca)
{
	offsetScale_ = sca;
}

MeshCollider* IFE::Collider::GetMeshCollider()
{
	return meshCollider_.get();
}

bool IFE::Collider::GetPushBackFlag()
{
	return pushBack_;
}

bool IFE::Collider::GetNoPushBackFlag()
{
	return notPushBack_;
}

bool IFE::Collider::GetGroundJudgeFlag()
{
	return groundJudge_;
}

void IFE::Collider::SetPushBackFlag(bool flag)
{
	pushBack_ = flag;
}

void IFE::Collider::SetNoPushBackFlag(bool flag)
{
	notPushBack_ = flag;
}

void IFE::Collider::SetGroundJudgeFlag(bool flag)
{
	groundJudge_ = flag;
}

void IFE::Collider::GetParentParms()
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

void IFE::Collider::LoadingComponent(nlohmann::json& json)
{
	colliderType_ = json["colliderType"];
	pushBack_ = json["pushBack"];
	attribute_ = json["attribute"];
	groundJudge_ = json["groundJudge"];
	notPushBack_ = json["notPushBack"];
}

#ifdef InverseEditorMode
#else
void IFE::Collider::OutputComponent(nlohmann::json& json)
{
	json["colliderType"] = colliderType_;
	json["pushBack"] = pushBack_;
	json["attribute"] = attribute_;
	json["groundJudge"] = groundJudge_;
	json["notPushBack"] = notPushBack_;
}

#include "ImguiManager.h"
void IFE::Collider::ComponentDebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();
	gui->CheckBoxGUI(&pushBack_, "pushBack");
	gui->CheckBoxGUI(&notPushBack_, "notPushBack");
	gui->CheckBoxGUI(&groundJudge_, "ground judgement");
	if (gui->NewTreeNode("Collider Type"))
	{
		int32_t num = (int32_t)colliderType_;
		gui->RadioButtonGUI("Sphere", &num, (int32_t)ColliderType::SPHERE);
		gui->RadioButtonGUI("Mesh", &num, (int32_t)ColliderType::MESH);
		colliderType_ = (ColliderType)num;
		gui->EndTreeNode();
	}
	if (gui->NewTreeNode("attribute"))
	{
		int32_t num = (int32_t)attribute_;
		gui->RadioButtonGUI("Landshape", &num, (int32_t)Attribute::LANDSHAPE);
		gui->RadioButtonGUI("Allies", &num, (int32_t)Attribute::ALLIES);
		gui->RadioButtonGUI("Enemys", &num, (int32_t)Attribute::ENEMYS);
		attribute_ = (uint16_t)num;
		gui->EndTreeNode();
	}
}
#endif
