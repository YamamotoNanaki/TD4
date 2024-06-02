#include "PostEffectManager.h"

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
