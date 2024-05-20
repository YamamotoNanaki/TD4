#include "EnemyHp.h"
#include "Object3D.h"
#include "Transform.h"
#include "ObjectManager.h"
#include "NormalEnemy.h"

void IFE::EnemyHp::Initialize()
{
	hp_ = MAX_HP;
	preHp_ = MAX_HP;
	decHp_ = 0;
	objectPtr_->SetColor({ 1,0,0,1 });
	objectPtr_->transform_->scale_ = { 2.0f,0.4f,1.0f };
	objectPtr_->transform_->eulerAngleDegrees_ = { 0, 0, 0 };
	objectPtr_->transform_->billbord_ = 2;
	isDead_ = false;
	isHit_ = false;
	hitTime_ = 0;
}

void IFE::EnemyHp::Update(Float3 pos_)
{
	//enemy‚Ì“ªã‚É•\Ž¦
	SetPos(pos_);
	ScaleCalc();
	//hitcool
	if (isHit_ == true) {
		hitTime_--;
		if (hitTime_ == 0) {
			isHit_ = false;
		}
	}
	//“G‚ªÁ‚¦‚½‚çÁ‚¦‚é
	if (isDead_ == true) {
		objectPtr_->Destroy();
	}
}

void IFE::EnemyHp::ScaleCalc()
{
	if (decHp_ >= 4) {
		transform_->scale_ -= {0.08f, 0, 0};
		decHp_-= 4;
	}
	else if (decHp_ >= 2) {
		transform_->scale_ -= {0.04f, 0, 0};
		decHp_-= 2;
	}
	else if (decHp_ >= 1) {
		transform_->scale_ -= {0.02f, 0, 0};
		decHp_ --;
	}
}

void IFE::EnemyHp::DecHp()
{
	if (isHit_ == false) {
		hp_ -= 25;
		decHp_ = preHp_ - hp_;
		preHp_ = hp_;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::EnemyHp::OneShot()
{
	if (isHit_ == false) {
		hp_ -= hp_;
		decHp_ = preHp_ - hp_;
		preHp_ = hp_;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::EnemyHp::Draw()
{
}

void IFE::EnemyHp::Finalize()
{
}

void IFE::EnemyHp::SetPos(Float3 pos_)
{
	transform_->position_ = { pos_.x - 2,pos_.y + 2,pos_.z };
}
