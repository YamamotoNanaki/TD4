#include "PlayerHp.h"

void IFE::PlayerHp::Update()
{

}

void IFE::PlayerHp::ScaleCalc(int8_t decHp_)
{
	if (decHp_ >= 4) {
		spritePtr_->transform_->scale2D_ -= {0.08f, 0};
		decHp_ -= 4;
	}
	else if (decHp_ >= 2) {
		spritePtr_->transform_->scale2D_ -= {0.04f, 0};
		decHp_ -= 2;
	}
	else if (decHp_ >= 1) {
		spritePtr_->transform_->scale2D_ -= {0.02f, 0};
		decHp_--;
	}
}
