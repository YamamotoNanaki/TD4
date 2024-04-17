#include "Emitter.h"
#include "Transform.h"
#include "Component.h"
#include "TextureManager.h"

using namespace IFE;
using namespace std;

IFE::Emitter::Emitter()
{
	SetEmitterPtr(this);
}

void IFE::Emitter::Initialize()
{
	if (GetComponent<TransformParticle>() == nullptr)AddComponent<TransformParticle>();
	transform = GetComponent<TransformParticle>();
	gp.CreateBasicParticleGraphicsPipeLine();
}

void IFE::Emitter::StaticInitialize()
{
	Particle::StaticInitialize();
}

void IFE::Emitter::SetComponent(Component* component)
{
	AddComponentBack<Component>(component);
}

void IFE::Emitter::SetComponentFront(Component* component)
{
	AddComponent<Component>(component);
}

void IFE::Emitter::SetTexture(std::string texName)
{
	this->tex = TextureManager::Instance()->GetTexture(texName);
}

void IFE::Emitter::Update()
{
	if (!isActive)return;
	ComponentManagerEmitter::Update();
	particles.remove_if([](unique_ptr<Particle>& obj) {return obj->deleteFlag; });
	for (std::unique_ptr<Particle>& itr : particles)
	{
		itr->Update();
	}
}

//void IFE::Emitter::DrawBefore()
//{
//}

void IFE::Emitter::Draw()
{
	if (!isActive)return;
	if (!DrawFlag)return;
	Particle::DrawBefore();
	gp.SetDrawBlendMode();
	tex->SetTexture(2);
	for (std::unique_ptr<Particle>& itr : particles)
	{
		itr->Draw();
	}
}

IFE::Emitter::~Emitter()
{
	ComponentManagerEmitter::Finalize();
	particles.clear();
}

#ifdef _DEBUG
#include "ImguiManager.h"
void IFE::Emitter::DebugGUI(bool flagdelete)
{
	if (ImguiManager::Instance()->EmitterGUI(emitterName, flagdelete))
	{
		deleteFlag = true;
	}
}

void IFE::Emitter::ComponentGUI()
{
	std::function<void(Component*)> addFunc = [&](Component* com)
	{
		SetComponentFront(com);
	};
	std::function<void(void)>f = [&]()
	{
		ComponentManagerEmitter::DebugGUI();
	};
	std::function<void(std::string)>texFunc = [&](std::string name)
	{
		tex = TextureManager::Instance()->GetTexture(name);
	};
	ImguiManager::Instance()->ComponentGUI2D(emitterName, f, addFunc, texFunc);
}

void IFE::Emitter::OutputScene()
{
	ComponentManagerEmitter::OutputScene(emitterName);
}

void IFE::Emitter::DebugUpdate()
{
	ComponentManagerEmitter::DebugUpdate();
}
#endif
