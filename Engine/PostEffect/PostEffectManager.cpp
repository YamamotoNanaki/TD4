#include "PostEffectManager.h"
#include "ComponentHelp.h"

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
	postEffects_.push_back(std::move(std::unique_ptr<IPostEffect>(std::move(ptr))));
}

void IFE::PostEffectManager::Draw()
{
	for (auto& itr : postEffects_)
	{
		itr->PostEffectDraw();
	}
}

void IFE::PostEffectManager::Update()
{
	for (auto& itr : postEffects_)
	{
		itr->PostEffectUpdate();
	}
}

void IFE::PostEffectManager::Initialize()
{
	postEffects_.push_back(std::move(std::make_unique<DefaultPostEffect>()));
	defaultPE = postEffects_.front().get();
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
	postEffects_.clear();
	defaultPE = nullptr;
}

void IFE::PostEffectManager::Finalize()
{
	Instance()->Reset();
}

IPostEffect* IFE::PostEffectManager::GetPostEffect(std::string name)
{
	for (auto& itr : postEffects_)
	{
		if (itr->name_ == name)return itr.get();
	}
	return nullptr;
}

void IFE::PostEffectManager::Loading()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("PostEffectManager");
	nlohmann::json js = jm->GetJsonData();
	for (auto& j : js)
	{
		std::string name;
		if (!jm->GetData(j, "name", name))continue;

		auto ptr = IPostEffectHelp::StringToPostEffect(name);
		if (!ptr)continue;
		ptr->PostEffectInitialize();
		postEffects_.push_back(std::move(std::unique_ptr<IPostEffect>(std::move(ptr))));
	}
}

#ifdef EditorMode
#include "ImguiManager.h"

void IFE::PostEffectManager::Output()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& j = jm->GetJsonData();
	uint32_t i = 0;
	for (auto& itr : postEffects_)
	{
		if (itr->name_ == "DefaultPostEffect")continue;
		j[i]["name"] = itr->name_;
		i++;
	}
	jm->Output("PostEffectManager");
}

void IFE::PostEffectManager::DebugGUI()
{
	auto gui = ImguiManager::Instance();
	gui->NewGUI("PostEffect");
	if (gui->CollapsingHeaderGUI(U8("’Ç‰Á")))
	{
		auto s = IPostEffectHelp::GetComponentList();
		if (gui->ButtonGUI("Add"))
		{
			Add(s);
		}
	}
	for (auto& itr : postEffects_)
	{
		itr->DebugGUI();
	}
	gui->EndGUI();
}
#endif