#include "Emitter.h"
#include "Transform.h"
#include "Component.h"
#include "TextureManager.h"
#include "GraphicsPipelineManager.h"
#include "ColorBuffer.h"

using namespace IFE;
using namespace std;

IFE::Emitter::Emitter()
{
	SetEmitterPtr(this);
}

void IFE::Emitter::Initialize()
{
	if (GetComponent<ColorBuffer>() == nullptr)AddComponent<ColorBuffer>();
	if (GetComponent<TransformParticle>() == nullptr)AddComponent<TransformParticle>();
	transform_ = GetComponent<TransformParticle>();
	gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("ParticleNormal");
	transformMap = transformCB_.GetCBMapObject();
	colorMap = colorCB_.GetCBMapObject();
}

void IFE::Emitter::CopyValue(Emitter* ptr)
{
	ptr->isActive_ = isActive_;
	ptr->deleteFlag_ = deleteFlag_;
	ptr->DrawFlag_ = DrawFlag_;
	ptr->emitterDeleteFlag_ = emitterDeleteFlag_;
	ptr->gp_ = gp_;
	ptr->particleMaxTime_ = particleMaxTime_;
	ptr->tex_ = tex_;
	ComponentManager::CopyValue(ptr);
	ptr->transform_ = ptr->GetComponent<TransformParticle>();
	ptr->SetTransform();
}

void IFE::Emitter::StaticInitialize()
{
	Particle::StaticInitialize();
}

void IFE::Emitter::SetComponent(std::unique_ptr<Component> component)
{
	AddComponentBack<Component>(std::move(component));
}

void IFE::Emitter::SetComponentFront(std::unique_ptr<Component> component)
{
	AddComponent<Component>(std::move(component));
}

void IFE::Emitter::SetTexture(const std::string& texName)
{
	this->tex_ = TextureManager::Instance()->GetTexture(texName);
}

Particle* IFE::Emitter::AddParticle(bool parentFlag)
{
	particles_.push_back(std::make_unique<Particle>());
	auto p = particles_.back().get();
	p->SetEmitter(this);
	p->Initialize(parentFlag);
	return p;
}

Particle* IFE::Emitter::AddParticle(Float3 pos)
{
	particles_.push_back(std::make_unique<Particle>());
	auto p = particles_.back().get();
	p->SetEmitter(this);
	p->Initialize(pos);
	return p;
}

Particle* IFE::Emitter::AddParticle()
{
	particles_.push_back(std::make_unique<Particle>());
	auto p = particles_.back().get();
	p->SetEmitter(this);
	p->Initialize(transform_->position_);
	return p;
}

void IFE::Emitter::Update()
{
	if (!isActive_)return;
	particles_.remove_if([&](unique_ptr<Particle>& obj) {return (obj->timer_ += IFETime::sDeltaTime_) > particleMaxTime_; });
	ComponentManager::Update();
	for (std::unique_ptr<Particle>& itr : particles_)
	{
		itr->Update();
	}
}

//void IFE::Emitter::DrawBefore()
//{
//}

void IFE::Emitter::Draw()
{
	if (!isActive_)return;
	if (!DrawFlag_)return;
	gp_->SetDrawBlendMode();
	Particle::DrawBefore();
	tex_->SetTexture(2);
	ComponentManager::Draw();
	uint32_t i = 0;
	if (!colorMap)
	{
		colorMap = colorCB_.GetCBMapObject();
	}
	if (!transformMap)
	{
		transformMap = transformCB_.GetCBMapObject();
	}
	for (std::unique_ptr<Particle>& itr : particles_)
	{
		if (i >= 400)break;
		itr->Draw();
		if (itr->GetColor().w == -1)
		{
			colorMap->color[i] = GetComponent<ColorBuffer>()->GetColor();
		}
		else
		{
			colorMap->color[i] = itr->GetColor();
		}
		transformMap->mat[i] = itr->GetMatrix().mat;
		transformMap->matBillboard[i] = itr->GetMatrix().matBillboard;
		i++;
	}
	transformCB_.SetConstBuffView(1);
	colorCB_.SetConstBuffView(0);
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, Particle::vb_.GetVBView());
	//描画コマンド
	commandList->DrawInstanced((UINT)Particle::vb_.GetSize(), particles_.size() < 400 ? UINT(particles_.size()) : 400, 0, 0);
}

IFE::Emitter::~Emitter()
{
	if (particles_.size() > 0)particles_.clear();
	ComponentManager::Finalize();
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
void IFE::Emitter::DebugGUI(bool flagdelete)
{
	if (ImguiManager::Instance()->EmitterGUI(emitterName_, flagdelete))
	{
		deleteFlag_ = true;
	}
}

void IFE::Emitter::ComponentGUI()
{
	ImguiManager* imgui = ImguiManager::Instance();
	std::function<void(std::unique_ptr<Component>)> addFunc = [&](std::unique_ptr<Component> com)
		{
			SetComponent(std::move(com));
		};
	std::function<void(void)> es = [&]()
		{
			imgui->CheckBoxGUI(&isActive_, "active");
			imgui->DragFloatGUI(&particleMaxTime_, "Particle Max Time", 0.1f);
		};
	std::function<void(void)>f = [&]()
		{
			imgui->CollapsingHeaderGUI("Emitter Setting", es);
			ComponentManager::DebugGUI();
		};
	std::function<void(const std::string&)>texFunc = [&](const std::string& name)
		{
			tex_ = TextureManager::Instance()->GetTexture(name);
		};
	imgui->ComponentGUI2D(emitterName_, f, addFunc);
}

void IFE::Emitter::OutputScene(nlohmann::json& j)
{
	j["name"] = emitterName_;
	j["maxTime"] = particleMaxTime_;
	j["texture"] = tex_->texName_;
	j["isActive"] = isActive_;
	uint32_t i = 0;
	for (auto& com : componentList_)
	{
		j["component"][i] = com->componentName_;
		i++;
	}
	ComponentManager::OutputScene(j);
}

void IFE::Emitter::DebugUpdate()
{
	ComponentManager::DebugUpdate();
}
#endif
void IFE::Emitter::LoadingScene(nlohmann::json& j)
{
	tex_ = TextureManager::Instance()->GetTexture(j["texture"]);
	particleMaxTime_ = j["maxTime"];
	isActive_ = j["isActive"];
	for (auto& com : j["component"])
	{
		ComponentManager::LoadingScene(j, com);
	}
}
