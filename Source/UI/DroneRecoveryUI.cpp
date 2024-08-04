#include "DroneRecoveryUI.h"
#include"SpriteManager.h"

void DroneRecoveryUI::Initialize()
{
	transform2D_->position2D_ = { 810.0f,800.0f };
	spritePtr_->anchorpoint_ = { 0.0f,0.5f };
	transform2D_->scale2D_.x = 0.0f;
	IFE::SpriteManager::Instance()->GetSpritePtr("droneRecoveryText")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("droneRecoveryUI")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("droneRecoveryUIGauge")->drawFlag_ = false;
	spritePtr_->TransferVertex();
}

void DroneRecoveryUI::Update()
{
}

void DroneRecoveryUI::Recovery(float time,float maxTime)
{
	transform2D_->scale2D_.x = (maxTime - time) * 1.0f;
}

void DroneRecoveryUI::SetDrawFlag(bool flag)
{
	IFE::SpriteManager::Instance()->GetSpritePtr("droneRecoveryText")->drawFlag_ = flag;
	IFE::SpriteManager::Instance()->GetSpritePtr("droneRecoveryUI")->drawFlag_ = flag;
	IFE::SpriteManager::Instance()->GetSpritePtr("droneRecoveryUIGauge")->drawFlag_ = flag;
}