#include "PostEffectManager.h"
#include "ComponentHelp.h"

//Œã‚ÅÁ‚¹
#include "EnemyHighlighting.h"
#include "RadialBlurPE.h"
#include "DronePostEffect.h"

using namespace IFE;

PostEffectManager* IFE::PostEffectManager::Instance()
{
	static PostEffectManager inst;
	return &inst;
}

void IFE::PostEffectManager::Add(std::string name, uint16_t num = 1)
{
	auto ptr = IPostEffectHelp::StringToPostEffect(name);
	ptr->SetInitParams(num);
	ptr->PostEffectInitialize();
	postEffects.push_back(std::make_unique<IPostEffect>(ptr));
}

void IFE::PostEffectManager::Draw()
{
	for (auto& itr : postEffects)
	{
		itr->PostEffectDraw();
	}
}

void IFE::PostEffectManager::Update()
{
	for (auto& itr : postEffects)
	{
		itr->PostEffectUpdate();
	}
}

void IFE::PostEffectManager::Initialize()
{
	postEffects.push_back(std::move(std::make_unique<DefaultPostEffect>()));
	defaultPE = postEffects.front().get();
	defaultPE->SetInitParams(2);
	defaultPE->PostEffectInitialize();
}

void IFE::PostEffectManager::ObjectDrawBefore()
{
	defaultPE->PostEffectDrawBefore();
}

void IFE::PostEffectManager::ObjectDrawAfter()
{
	defaultPE->PostEffectDrawAfter();
}

void IFE::PostEffectManager::Reset()
{
	postEffects.clear();
	defaultPE = nullptr;
}

void IFE::PostEffectManager::Finalize()
{
	Instance()->Reset();
}

IPostEffect* IFE::PostEffectManager::GetPostEffect(std::string name)
{
	for (auto& itr : postEffects)
	{
		if (itr->name_ == name)return itr.get();
	}
	return nullptr;
}

void IFE::PostEffectManager::Loading()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("EffekserManager");
	nlohmann::json js = jm->GetJsonData();
	for (auto& j : js)
	{
		std::string name;
		if (!jm->GetData(j, "name", name))continue;

		int16_t num;
		if (!jm->GetData(j, "initParam", num))continue;

		auto ptr = IPostEffectHelp::StringToPostEffect(name);
		ptr->SetInitParams(num);
		ptr->PostEffectInitialize();
	}
}

#ifdef EditorMode
#include "ImguiManager.h"

void IFE::PostEffectManager::Output()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& j = jm->GetJsonData();
	uint32_t i = 0;
	for (auto& itr : postEffects)
	{
		j[i]["name"] = itr->name_;
		j[i]["initParam"] = itr->GetTexSize();
		i++;
	}
	jm->Output("EffekserManager");
}

void IFE::PostEffectManager::DebugGUI()
{
	auto gui = ImguiManager::Instance();
	gui->NewGUI("PostEffect");
	if (gui->CollapsingHeaderGUI(U8("’Ç‰Á")))
	{
		auto s = IPostEffectHelp::GetComponentList();
		int32_t num;
		gui->DragIntGUI(&num, "output texture num", 1, 1);
		if (gui->ButtonGUI("Add"))
		{
			Add(s, uint16_t(num));
		}
	}
	for (auto& itr : postEffects)
	{
		itr->DebugGUI();
	}
	gui->EndGUI();
}
#endif