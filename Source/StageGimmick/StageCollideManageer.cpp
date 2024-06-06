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
		if (itr->GetObjectName().find("ground") != std::string::npos || itr->GetObjectName().find("wall") != std::string::npos || itr->GetObjectName().find("roof") != std::string::npos || itr->GetObjectName().find("box") != std::string::npos)
		{
			if (!itr->GetComponent<Collider>())
			{
				itr->AddComponentBack<Collider>();
				auto col = itr->GetComponent<Collider>();
				auto c = col->AddCollider();
				c->SetColliderType(ColliderType::OBB);
				c->SetNoPushBackFlag(false);
				c->SetPushBackFlag(false);
				c->attribute_ = uint16_t(Attribute::LANDSHAPE);
			}
			if (itr->GetObjectName().find("wall") != std::string::npos)
			{
				stages.push_back(itr.get());
			}
		}

		if (itr->GetObjectName().find("roof") != std::string::npos)
		{
			itr->DrawFlag_ = true;
		}
	}
}

void StageCollideManageer::Update()
{
	for (auto& itr : stages)
	{
		itr->DrawFlag_ = true;
	}
}
