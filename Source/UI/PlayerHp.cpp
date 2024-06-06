#include "PlayerHp.h"

void IFE::PlayerHp::Initialize()
{
	transform2D_->position2D_ = {50.0f,950.0f};
	spritePtr_->anchorpoint_ = { 0.0f,0.5f };
	spritePtr_->TransferVertex();
}

void IFE::PlayerHp::Update()
{

}

void IFE::PlayerHp::ScaleCalc(int8_t decHp_)
{
	if (decHp_ >= 4) {
		spritePtr_->transform_->scale2D_ -= {0.08f, 0};
		/*decHp_ -= 4;*/
	}
	else if (decHp_ >= 2) {
		spritePtr_->transform_->scale2D_ -= {0.04f, 0};
		/*decHp_ -= 2;*/
	}
	else if (decHp_ >= 1) {
		spritePtr_->transform_->scale2D_ -= {0.02f, 0};
		/*decHp_--;*/
	}
}
