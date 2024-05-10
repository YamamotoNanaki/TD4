#include "Component.h"
#include "Transform.h"
#include "Object3D.h"
#include "Sprite.h"
#include "Emitter.h"
#include "Particle.h"
#include "Collider.h"
#include "Camera.h"

void IFE::Component::SetTransform()
{
	if (objectPtr_ != nullptr)transform_ = objectPtr_->GetComponent<Transform>();
	else if (spritePtr_ != nullptr)transform2D_ = spritePtr_->GetComponent<Transform2D>();
	else if (particlePtr_ != nullptr)transformParticle_ = particlePtr_->GetComponent<TransformParticle>();
	else if (emitterPtr_ != nullptr)transformParticle_ = emitterPtr_->GetComponent<TransformParticle>();
	else if (cameraPtr_ != nullptr)transformCamera_ = cameraPtr_->GetComponent<TransformCamera>();
}

std::string IFE::Component::GetComponentName()
{
	return componentName_;
}

void IFE::Component::INITIALIZE()
{
	if (objectPtr_ != nullptr)transform_ = objectPtr_->GetComponent<Transform>();
	else if (spritePtr_ != nullptr)transform2D_ = spritePtr_->GetComponent<Transform2D>();
	else if (particlePtr_ != nullptr)transformParticle_ = particlePtr_->GetComponent<TransformParticle>();
	else if (emitterPtr_ != nullptr)transformParticle_ = emitterPtr_->GetComponent<TransformParticle>();
	else if (cameraPtr_ != nullptr)transformCamera_ = cameraPtr_->GetComponent<TransformCamera>();
	Initialize();
}

IFE::Component::~Component()
{
}

IFE::Component::Component(ComponentManager* c)
{
	if (c->objectPtr_ != nullptr)
	{
		objectPtr_ = c->objectPtr_;
		transform_ = objectPtr_->GetComponent<Transform>();
	}
	else if (c->spritePtr_ != nullptr)
	{
		spritePtr_ = c->spritePtr_;
		transform2D_ = spritePtr_->GetComponent<Transform2D>();
	}
	else if (c->particlePtr_ != nullptr)
	{
		particlePtr_ = c->particlePtr_;
		transformParticle_ = particlePtr_->GetComponent<TransformParticle>();
	}
	else if (c->emitterPtr_ != nullptr)
	{
		emitterPtr_ = c->emitterPtr_;
		transformParticle_ = emitterPtr_->GetComponent<TransformParticle>();
	}
	else if (c->cameraPtr_ != nullptr)
	{
		cameraPtr_ = c->cameraPtr_;
		transformCamera_ = cameraPtr_->GetComponent<TransformCamera>();
	}
}

void IFE::Component::SetComponentName(const std::string& n)
{
	componentName_ = n;
}

void IFE::Component::SetComponents(ComponentManager* cm)
{
	if (cm->objectPtr_ != nullptr)
	{
		objectPtr_ = cm->objectPtr_;
		transform_ = objectPtr_->GetComponent<Transform>();
	}
	else if (cm->spritePtr_ != nullptr)
	{
		spritePtr_ = cm->spritePtr_;
		transform2D_ = spritePtr_->GetComponent<Transform2D>();
	}
	else if (cm->particlePtr_ != nullptr)
	{
		particlePtr_ = cm->particlePtr_;
		transformParticle_ = particlePtr_->GetComponent<TransformParticle>();
	}
	else if (cm->emitterPtr_ != nullptr)
	{
		emitterPtr_ = cm->emitterPtr_;
		transformParticle_ = emitterPtr_->GetComponent<TransformParticle>();
	}
	else if (cm->cameraPtr_ != nullptr)
	{
		cameraPtr_ = cm->cameraPtr_;
		transformCamera_ = cameraPtr_->GetComponent<TransformCamera>();
	}
}

bool IFE::Component::GetComponentDeleteFlag()
{
	return componentDeleteFlag_;
}

IFE::Object3D* IFE::Component::GetObjectPtr()
{
	return objectPtr_;
}

IFE::Sprite* IFE::Component::GetSpritePtr()
{
	return spritePtr_;
}

IFE::Emitter* IFE::Component::GetEmitterPtr()
{
	return emitterPtr_;
}

IFE::Particle* IFE::Component::GetParticlePtr()
{
	return particlePtr_;
}

IFE::Camera* IFE::Component::GetCameraPtr()
{
	return cameraPtr_;
}

//void IFE::Component::OnColliderHit(ADXCollider* myCol, ADXCollider* col)
//{
//	myCol; col;
//}

void IFE::Component::Copy(Component* c) { c; }

void IFE::Component::OnColliderHit(ColliderComponent* myCollider, ColliderComponent* hitCollider)
{
	myCollider, hitCollider;
}

void IFE::Component::CopyValue(Component* component)
{
	component->componentName_ = componentName_;
	Copy(component);
}

void IFE::Component::CopyValueComponent(Component* component) { component; }

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
void IFE::Component::DebugGUI(uint32_t num)
{
	std::function<void(void)>func = [&]()
		{
			ImguiManager::Instance()->TextGUI(componentName_.c_str());
			ComponentDebugGUI();
		};
	std::function<void(void)>deleteFunc = [&]()
		{
			componentDeleteFlag_ = true;
		};
	std::string s = std::to_string(num) + " : " + componentName_;
	ImguiManager::Instance()->ComponentGUI(func, deleteFunc);
}

void IFE::Component::OutputScene(nlohmann::json& j)
{
	OutputComponent(j[componentName_]);
}
void IFE::Component::OutputComponent(nlohmann::json& j)
{
	j;
}
#endif

void IFE::Component::LoadingScene(nlohmann::json& j)
{
	LoadingComponent(j);
}

void IFE::Component::LoadingComponent(nlohmann::json& j)
{
	j;
}
