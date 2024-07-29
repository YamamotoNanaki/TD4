#include "CameraChange.h"
#include "ObjectManager.h"
#include "SpriteManager.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "Rand.h"
#include "WindowsAPI.h"

using namespace IFE;

void CameraChange::Initialize()
{
	ccp_ = ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>()->GetCameraChangeParam();
}

void CameraChange::Update()
{
	if (ccp_->cameraChange)
	{
		endFlag_ = false;
		if (ccp_->cameraChangeTimer < ccp_->changeTime)
		{
			Add();
			UpdateSquare();
		}
		else
		{
			UpdateEnd();
		}
	}
	else if (!endFlag_)
	{
		End();
		endFlag_ = true;
	}
}

void CameraChange::Add()
{
	float y = IFERand::GetRandF(float(0 + 100), float(WindowsAPI::Instance()->winHeight_ - 100));
	float x = float(sprite_.size() * 30) + IFERand::GetRandF(-10, 30);
	auto s = SpriteManager::Instance()->AddInitialize("CameraChange", "white");
	s->transform_->scale2D_ = { 0,0 };
	s->transform_->position2D_ = { x,y };
	s->transform_->rotation2D_ = 45;
	s->order_ = 253;
	sprite_.push_back(s);
}

void CameraChange::UpdateSquare()
{
	for (size_t i = 0; i < sprite_.size(); i++)
	{
		sprite_[i]->transform_->scale2D_ += 0.15f;
	}
}

void CameraChange::UpdateEnd()
{
	float alpha = IFE::Lerp(1, 0, ccp_->cameraChangeMaxTime - ccp_->changeTime, ccp_->cameraChangeTimer - ccp_->changeTime);
	for (size_t i = 1; i < sprite_.size(); i++)
	{
		sprite_[i]->Destroy();
	}
	sprite_[0]->transform_->scale2D_ = { float(WindowsAPI::Instance()->winWidth_ / 2),float(WindowsAPI::Instance()->winHeight_ / 2) };
	sprite_[0]->transform_->position2D_ = { float(WindowsAPI::Instance()->winWidth_ / 2),float(WindowsAPI::Instance()->winHeight_ / 2) };
	sprite_[0]->transform_->rotation2D_ = 0;
	sprite_[0]->GetComponent<ColorBuffer>()->SetAlpha(alpha);
}

void CameraChange::End()
{
	for (size_t i = 0; i < sprite_.size(); i++)
	{
		sprite_[i]->Destroy();
	}
	sprite_.clear();
}

