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
		c->SetNoPushBackFlag(true);
		c->SetPushBackFlag(false);
		c->SetOffsetPosition(transform_->position_);
		c->SetOffsetScale(transform_->position_);
		col->GetCollider(0)->active_ = false;
	}
	
	objectPtr_->DrawFlag_ = false;
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
		//‰Ÿ‚µ–ß‚µˆ—‚ð‘‚­
		PushBack(hitCollider, hitCollider->reject_);
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


void DroneKeepoutZoneObject::PushBack(IFE::ColliderCore* HitCol, const IFE::Vector3& reject)
{
	// ’n–Ê”»’è‚µ‚«‚¢’l
	static const float threshold = cosf(IFE::ConvertToRadians(30.0f));
	static const IFE::Vector3 up = { 0,1,0 };
	
	IFE::Vector3 rejectDir = Vector3Normalize(reject);
	float cos = Vector3Dot(rejectDir, up);
	if (-threshold < cos && cos < threshold)
	{
		HitCol->transform_->MovePushBack(reject * 2);
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