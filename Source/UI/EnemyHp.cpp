#include "EnemyHp.h"
#include "Object3D.h"
#include "Transform.h"

void IFE::EnemyHp::Initialize()
{
	hp_ = MAX_HP;
	objectPtr_->SetColor({ 1,0,0,1 });
	objectPtr_->transform_->scale_ = { 1.0f,1.0f,0.5f };
	objectPtr_->transform_->eulerAngleDegrees_ = { 270, 0, 0 };
	objectPtr_->transform_->billbord_ = 1;
}

void IFE::EnemyHp::Update()
{
	//“G‚ªÁ‚¦‚½‚çÁ‚¦‚é
	if (isDead == true) {
		objectPtr_->Destroy();
	}
}

void IFE::EnemyHp::Draw()
{
}

void IFE::EnemyHp::Finalize()
{
}
