#include "PostEffectManager.h"

//Œã‚ÅÁ‚¹
#include "EnemyHighlighting.h"

using namespace IFE;

PostEffectManager* IFE::PostEffectManager::Instance()
{
	static PostEffectManager inst;
	return &inst;
}

void IFE::PostEffectManager::Draw()
{
	for (auto& itr : postEffects)
	{
		if (itr->drawFlag_)
		{
			itr->PostEffectDraw();
		}
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
	defaultPE->PostEffectInitialize();
	postEffects.push_back(std::move(std::make_unique<EnemyHighlighting>()));
	postEffects.back()->SetInitParams(2);
	postEffects.back()->PostEffectInitialize();
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
	Reset();
}

IPostEffect* IFE::PostEffectManager::GetPostEffect(std::string name)
{
	for (auto& itr : postEffects)
	{
		if (itr->name_ == name)return itr.get();
	}
	return nullptr;
}
