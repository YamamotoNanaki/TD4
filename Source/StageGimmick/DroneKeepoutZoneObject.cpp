#include "DroneKeepoutZoneObject.h"
#include "Collider.h"
#include "Object3D.h"
#include "PlayerDrone.h"
#include "Transform.h"

void DroneKeepoutZoneObject::Initialize()
{
	if (!objectPtr_->GetComponent<IFE::Collider>())
	{
		objectPtr_->AddComponentBack<IFE::Collider>();
		auto col = objectPtr_->GetComponent<IFE::Collider>();
		auto c = col->AddCollider();
		c->SetColliderType(IFE::ColliderType::OBB);
		c->SetNoPushBackFlag(false);
		c->SetPushBackFlag(false);
		c->SetOffsetPosition(transform_->position_);
		c->SetOffsetScale(transform_->position_);
	}
}

void DroneKeepoutZoneObject::Update()
{

}

void DroneKeepoutZoneObject::Draw()
{

}

void DroneKeepoutZoneObject::Finalize()
{

}

void DroneKeepoutZoneObject::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	if (hitCollider->GetObjectPtr()->GetComponent<PlayerDrone>())
	{
		//�����߂�����������
		//PushBack(myCollider,hitCollider)
	}
	myCollider;
	hitCollider;
}

#ifdef EditorMode
#include "ImguiManager.h"
void DroneKeepoutZoneObject::ComponentDebugGUI() 
{

}

void DroneKeepoutZoneObject::OutputComponent(nlohmann::json& json)
{
	json;
}

#endif

void DroneKeepoutZoneObject::LoadingComponent(nlohmann::json& json)
{
	json;
}


void DroneKeepoutZoneObject::PushBack(IFE::ColliderCore* colA, IFE::ColliderCore* colB, const IFE::Vector3& reject)
{
	// �n�ʔ��肵�����l
	static const float threshold = cosf(IFE::ConvertToRadians(30.0f));
	static const IFE::Vector3 up = { 0,1,0 };
	if (colA->GetNoPushBackFlag() || colB->GetNoPushBackFlag())
	{
		return;
	}
	if (colA->GetPushBackFlag() && colB->GetPushBackFlag())
	{
		IFE::Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colA->transform_->MovePushBack(reject);
			colB->transform_->MovePushBack(-reject);
		}
	}
	else if (colA->GetPushBackFlag())
	{
		IFE::Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colA->transform_->MovePushBack(reject * 2);
		}
	}
	else if (colB->GetPushBackFlag())
	{
		IFE::Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colB->transform_->MovePushBack(-reject * 2);
		}
	}
}

void DroneKeepoutZoneObject::ChangeActiveZone(bool flag)
{
	if (flag)
	{
		auto col = objectPtr_->GetComponent<IFE::Collider>();
		col->GetCollider(0)->active_ = true;
		objectPtr_->DrawFlag_ = true;
	}
	else
	{
		auto col = objectPtr_->GetComponent<IFE::Collider>();
		col->GetCollider(0)->active_ = false;
		objectPtr_->DrawFlag_ = false;
	}
}