#include "EnemyHp.h"
#include "Object3D.h"
#include "Transform.h"
#include "ObjectManager.h"
#include "NormalEnemy.h"

void IFE::EnemyHp::Initialize()
{
	hp_ = MAX_HP;
	objectPtr_->SetColor({ 1,0,0,1 });
	objectPtr_->transform_->scale_ = { 1.0f,1.0f,0.4f };
	objectPtr_->transform_->eulerAngleDegrees_ = { 270, 0, 0 };
	objectPtr_->transform_->billbord_ = 2;
	isDead_ = false;
	isHit_ = false;
	hitTime_ = 0;
}

void IFE::EnemyHp::Update(Float3 pos_)
{
	//enemy‚Ì“ªã‚É•\Ž¦
	SetPos(pos_);
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

void IFE::EnemyHp::DecHp()
{
	if (isHit_ == false) {
		hp_ -= 25;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::EnemyHp::OneShot()
{
	if (isHit_ == false) {
		hp_ -= hp_;
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
	transform_->position_ = { pos_.x,pos_.y + 2,pos_.z };
}
