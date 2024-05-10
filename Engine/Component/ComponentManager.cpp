#include "ComponentManager.h"
#include "ComponentHelp.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Emitter.h"
#include "Particle.h"
#include "Collider.h"

void IFE::ComponentManager::Initialize()
{
	for (auto& itr : componentList_)
	{
		if (itr)itr->INITIALIZE();
	}
}

void IFE::ComponentManager::Update()
{
	componentList_.remove_if([](std::unique_ptr<Component>& com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList_)
	{
		if (itr)itr->Update();
	}
}

void IFE::ComponentManager::Draw()
{
	for (auto& itr : componentList_)
	{
		if (itr)itr->Draw();
	}
}

void IFE::ComponentManager::Finalize()
{
	componentList_.clear();
}

void IFE::ComponentManager::SetTransform()
{
	for (auto& itr : componentList_)
	{
		itr->SetTransform();
	}
}

void IFE::ComponentManager::SetObjectPtr(Object3D* p)
{
	objectPtr_ = p;
}

std::vector<std::string> IFE::ComponentManager::GetAllComponentName()
{
	std::vector<std::string>s;

	for (auto& itr : componentList_)
	{
		s.push_back(itr->GetComponentName());
	}
	return s;
}

void IFE::ComponentManager::OnColliderHit(ColliderComponent* myCollider, ColliderComponent* hitCollider)
{
	for (auto& itr : componentList_)
	{
		itr->OnColliderHit(myCollider, hitCollider);
	}
}

void IFE::ComponentManager::CopyValue(ComponentManager* ptr)
{
	for (auto& itr : componentList_)
	{
		if (itr)
		{
			std::string s = itr->GetComponentName();
			auto c = StringToComponent(s);
			c->CopyValue(itr.get());
			ptr->AddComponentBack<Component>(std::unique_ptr<Component>(c));
		}
	}
}

#ifdef InverseEditorMode
#else
void IFE::ComponentManager::DebugGUI()
{
	uint32_t num = 0;
	for (auto& itr : componentList_)
	{
		if (itr)itr->DebugGUI(num);
		num++;
	}
}

void IFE::ComponentManager::OutputScene(nlohmann::json& j)
{
	for (auto& itr : componentList_)
	{
		if (itr)itr->OutputScene(j);
	}
}

void IFE::ComponentManager::DebugUpdate()
{
	componentList_.remove_if([](std::unique_ptr<Component>& com) {return com->GetComponentDeleteFlag(); });
	for (auto& itr : componentList_)
	{
		std::string name = itr->GetComponentName();
		if (name == "Transform");
		else if (name == "TransferGeometryBuffer");
		else if (name == "Material");
		else if (name.find("_Model") != std::string::npos);
		else if (name == "ColorBuffer");
		else
		{
			continue;
		}
		if (itr)itr->Update();
	}
}
#endif

void IFE::ComponentManager::LoadingScene(nlohmann::json& j, const std::string& comName)
{
	auto com = StringToComponent(comName);
	com->LoadingScene(j[comName]);
	AddComponentBack<Component>(std::unique_ptr<Component>(com));
}

std::string IFE::ComponentManager::SetName(const std::string& name)
{
	std::string n = name;
	if (n.starts_with("class "))
	{
		n.replace(0, 6, "");
	}
	if (n.starts_with("struct "))
	{
		n.replace(0, 7, "");
	}
	if (n.starts_with("IFE::"))
	{
		n.replace(0, 5, "");
	}
	return n;
}

void IFE::ComponentManager::SetSpritePtr(Sprite* p)
{
	spritePtr_ = p;
}

void IFE::ComponentManager::SetEmitterPtr(Emitter* emitterPtr)
{
	emitterPtr_ = emitterPtr;
}

void IFE::ComponentManager::SetParticlePtr(Particle* particlePtr)
{
	particlePtr_ = particlePtr;
}

void IFE::ComponentManager::SetCameraPtr(Camera* cameraPtr)
{
	cameraPtr_ = cameraPtr;
}
