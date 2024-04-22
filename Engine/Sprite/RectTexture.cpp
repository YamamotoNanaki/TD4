#include "RectTexture.h"
#include "Sprite.h"

IFE::RectTexture::~RectTexture()
{
	spritePtr_->SetTextureRect({ 0,0 }, initSize_);
}

void IFE::RectTexture::Update()
{
	if (spritePtr_->texBase_ != texBase_ || spritePtr_->texSize_ != texSize_)
	{
		spritePtr_->SetTextureRect(texBase_, texSize_);
	}
}

void IFE::RectTexture::Initialize()
{
	if (!spritePtr_->tex_)return;
	if (spritePtr_->tex_->texbuff_ && initSize_ == Float2{ 0,0 })initSize_ = { (float)spritePtr_->tex_->texbuff_->GetDesc().Width,(float)spritePtr_->tex_->texbuff_->GetDesc().Height };
	if (init)
	{
		texBase_ = spritePtr_->texBase_;
		texSize_ = initSize_;
	}
	else
	{
		spritePtr_->SetTextureRect(texBase_, texSize_);
	}
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
void IFE::RectTexture::ComponentDebugGUI()
{
	auto im = ImguiManager::Instance();
	im->DragFloat2GUI(&texBase_, "texture upper left", 1);
	im->DragFloat2GUI(&texSize_, "texture size", 1);
	if (im->ButtonGUI("Reset"))
	{
		texBase_ = { 0,0 };
		texSize_ = initSize_;
	}
	if (spritePtr_->texBase_ != texBase_ || spritePtr_->texSize_ != texSize_)
	{
		spritePtr_->SetTextureRect(texBase_, texSize_);
	}
}

void IFE::RectTexture::OutputComponent(nlohmann::json& json)
{
	auto j = JsonManager::Instance();
	j->OutputFloat2(json["base"], texBase_);
	j->OutputFloat2(json["size"], texSize_);
}

#endif
void IFE::RectTexture::LoadingComponent(nlohmann::json& json)
{
	auto j = JsonManager::Instance();
	init = false;
	texBase_ = j->InputFloat2(json["base"]);
	texSize_ = j->InputFloat2(json["size"]);
}


