#include "DefaultPostEffect.h"
#include "GraphicsPipelineManager.h"

void IFE::DefaultPostEffect::Draw()
{
	tex_[0]->SetTexture(1);
}

void IFE::DefaultPostEffect::Update()
{
}

void IFE::DefaultPostEffect::Initialize()
{
	name_ = "DefaultPostEffect";
	gp_ = GraphicsPipelineManager::Instance()->CreateDefaultPostEffectPipeLine();
}

void IFE::DefaultPostEffect::DrawBefore()
{
}

void IFE::DefaultPostEffect::DrawAfter()
{
}
