#include "StageCollideManageer.h"
#include "Object3D.h"
#include "ObjectManager.h"
#include "Collider.h"

using namespace IFE;

void StageCollideManageer::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	ObjectManager* objm = ObjectManager::Instance();
	auto& list = objm->GetObjList();

	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("ground") != std::string::npos || itr->GetObjectName().find("wall") != std::string::npos || itr->GetObjectName().find("roof") != std::string::npos)
		{
			if (itr->GetComponent<Collider>())continue;
			itr->AddComponentBack<Collider>();
			auto col = itr->GetComponent<Collider>();
			auto c = col->AddCollider();
			c->SetColliderType(ColliderType::OBB);
			c->SetNoPushBackFlag(true);
			c->SetPushBackFlag(true);
		}

		if (itr->GetObjectName().find("roof") != std::string::npos)
		{
			itr->DrawFlag_ = true;
		}
	}
}

void StageCollideManageer::Update()
{
	objectPtr_->DrawFlag_ = true;
}

void StageCollideManageer::OnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	if (hitCollider->cameraPtr_)
	{
		objectPtr_->DrawFlag_ = false;
	}
	myCollider;
}
