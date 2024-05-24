#include "AnimationTexture.h"
#include "Sprite.h"

void IFE::AnimationTexture::Update()
{
	if (sheetsNumX_ != oldSheetsNumX || sheetsNumY_ != oldSheetsNumY || nowNum_ != oldNowNum || flipX_ != oldFlipX || flipY_ != oldFlipY)
	{
		spritePtr_->SetAnimation(sheetsNumX_, sheetsNumY_, nowNum_, flipX_, flipY_);
		oldSheetsNumX = sheetsNumX_, oldSheetsNumY = sheetsNumY_, oldNowNum = nowNum_, oldFlipX = flipX_, oldFlipY = flipY_;
	}
}

IFE::AnimationTexture::~AnimationTexture()
{
	spritePtr_->TransferVertex();
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
void IFE::AnimationTexture::ComponentDebugGUI()
{
	auto im = ImguiManager::Instance();
	Int2 i = { sheetsNumX_,sheetsNumY_ };
	int32_t n = (int32_t)nowNum_;
	const static uint16_t max = uint16_t(-1);
	im->DragInt2GUI(&i, "sheetsNum", 1, 0, int32_t(max));
	im->DragIntGUI(&n, "nowNum_");
	im->CheckBoxGUI(&flipX_, "flipX");
	im->CheckBoxGUI(&flipY_, "flipY");
	sheetsNumX_ = (uint16_t)i.x;
	sheetsNumY_ = (uint16_t)i.y;
	nowNum_ = uint16_t(n);
	if (sheetsNumX_ != oldSheetsNumX || sheetsNumY_ != oldSheetsNumY || nowNum_ != oldNowNum || flipX_ != oldFlipX || flipY_ != oldFlipY)
	{
		spritePtr_->SetAnimation(sheetsNumX_, sheetsNumY_, nowNum_, flipX_, flipY_);
		oldSheetsNumX = sheetsNumX_, oldSheetsNumY = sheetsNumY_, oldNowNum = nowNum_, oldFlipX = flipX_, oldFlipY = flipY_;
	}
}

void IFE::AnimationTexture::OutputComponent(nlohmann::json& json)
{
	json["sheetsNumX"] = sheetsNumX_;
	json["sheetsNumY"] = sheetsNumY_;
	json["nowNum"] = nowNum_;
	json["flipX"] = flipX_;
	json["flipY"] = flipY_;
}
#endif

void IFE::AnimationTexture::LoadingComponent(nlohmann::json& json)
{
	sheetsNumX_ = json["sheetsNumX"];
	sheetsNumY_ = json["sheetsNumY"];
	nowNum_ = json["nowNum"];
	flipX_ = json["flipX"];
	flipY_ = json["flipY"];
}
