#include "StageCollideManageer.h"
#include "Object3D.h"
#include "ObjectManager.h"
#include "Collider.h"
#include "GraphicsPipelineManager.h"
#include "Material.h"
#include "TextureManager.h"

using namespace IFE;

void StageCollideManageer::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	ObjectManager* objm = ObjectManager::Instance();
	auto& list = objm->GetObjList();

	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("ground") != std::string::npos || itr->GetObjectName().find("wall") != std::string::npos || itr->GetObjectName().find("roof") != std::string::npos || itr->GetObjectName().find("box") != std::string::npos || itr->GetObjectName().find("lightWall") != std::string::npos)
		{
			auto mat = itr->GetComponent<Material>();
			mat->SetTexture(TextureManager::Instance()->GetTexture("white"));
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
				mat->SetColor255({ 35,35,35,255 });
				mat->ambient_ = { 0.3f,0.3f,0.3f };
				mat->diffuse_ = { 0.7f,0.7f,0.7f };
				mat->specular_ = { 1.0f,1.0f,1.0f };
				stages.push_back(itr.get());
			}
			if (itr->GetObjectName().find("lightWall") != std::string::npos)
			{
				mat->SetColor255({ 74,161,142,255 });
				mat->ambient_ = { 1,1,1 };
				mat->diffuse_ = { 0,0,0 };
				mat->specular_ = { 0,0,0 };
				mat->bloom_ = true;
				stages.push_back(itr.get());
			}
			if (itr->GetObjectName().find("ground") != std::string::npos)
			{
				mat->SetColor255({ 141,141,141,255 });
				mat->ambient_ = { 0.3f,0.3f,0.3f };
				mat->diffuse_ = { 0.6f,0.6f,0.6f };
				mat->specular_ = { 0.7f,0.7f,0.7f };
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
	//for (auto& itr : stages)
	//{
	//	itr->DrawFlag_ = true;
	//}
}
