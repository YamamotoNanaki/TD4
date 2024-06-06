#include "PlayerHp.h"

void IFE::PlayerHp::Initialize()
{
	transform2D_->position2D_ = { 50.0f,950.0f };
	spritePtr_->anchorpoint_ = { 0.0f,0.5f };
	spritePtr_->TransferVertex();
}

void IFE::PlayerHp::Update()
{

}

void IFE::PlayerHp::ScaleCalc(uint8_t hp, uint8_t decHp, float hitTime, float maxTime)
{
	transform2D_->scale2D_.x = static_cast<float>(EaseOutQuint(maxTime - hitTime, (hp + decHp) * 0.5f, hp * 0.5f, maxTime));
}

void IFE::PlayerHp::SetHp(uint8_t hp)
{
	transform2D_->scale2D_.x = hp * 0.5f;
}