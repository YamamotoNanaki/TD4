#include "RadialBlurPE.h"
#include "GraphicsPipelineManager.h"
#include "PostEffectManager.h"

void IFE::RadialBlurPE::SetCenter(Float2 center)
{
	buff_->center = center;
}

void IFE::RadialBlurPE::SetBlurAmount(float blurAmount)
{
	buff_->blurAmount = blurAmount;
}

void IFE::RadialBlurPE::Draw()
{
	tex_[0]->SetTexture(0);
	cb_->SetConstBuffView(1);
}

void IFE::RadialBlurPE::Update()
{
	PostEffectDrawBefore();
	Sprite::DrawBefore();
	PostEffectManager::Instance()->GetDefaultPE()->ForcedDraw();
	PostEffectDrawAfter();
}

void IFE::RadialBlurPE::Initialize()
{
	name_ = "RadialBlur";
	gp_ = GraphicsPipelineManager::Instance()->CreatePostEffectPipeLine("RadialBlur", 1, 1, 1);
	drawFlag_ = false;
	updateFlag_ = true;
	cb_ = std::make_unique<IFE::ConstBuffer<RadialCB>>();
	buff_ = cb_->GetCBMapObject();
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::RadialBlurPE::PostEffectDebugGUI()
{
	ImguiManager::Instance()->DragFloat2GUI(&buff_->center, "center", 0.01f, 0, 1);
	ImguiManager::Instance()->DragFloatGUI(&buff_->blurAmount, "amount", 0.01f, -1, 1);
}
#endif
