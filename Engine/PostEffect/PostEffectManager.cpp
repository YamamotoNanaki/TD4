#include "PostEffectManager.h"

using namespace IFE;

PostEffectManager* IFE::PostEffectManager::Instance()
{
	static PostEffectManager inst;
	return &inst;
}

void IFE::PostEffectManager::Draw()
{
	postEffects.back()->PostEffectDraw();
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
}

void IFE::PostEffectManager::ObjectDrawBefore()
{
	defaultPE->PostEffectDrawBefore();
}

void IFE::PostEffectManager::ObjectDrawAfter()
{
	defaultPE->PostEffectDrawAfter();
}

void IFE::PostEffectManager::Finalize()
{
	postEffects.clear();
	defaultPE = nullptr;
}
