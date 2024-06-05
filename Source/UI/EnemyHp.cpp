#include "EnemyHp.h"
#include "Object3D.h"
#include "Transform.h"
#include "ObjectManager.h"
#include "NormalEnemy.h"
#include "Player.h"
#include "SpriteManager.h"

void IFE::EnemyHp::Initialize()
{
	hp_ = 100;
	isDead = false;
	objectPtr_->SetColor({ 1,0,0,1 });
	objectPtr_->transform_->scale_ = { 2.0f,0.4f,1.0f };
	objectPtr_->transform_->eulerAngleDegrees_ = { 0, 0, 0 };
	objectPtr_->transform_->billbord_ = 2;
}

void IFE::EnemyHp::Update(Float3 pos_,int8_t hp, int8_t& decHp_)
{
	hp_ = hp;
	//enemyの頭上に表示
	SetPos(pos_);
	ScaleCalc(decHp_);
	if (hp_ == 0 && decHp_ == 0) {
		isDead = true;
	}
}

void IFE::EnemyHp::IconUpdate(Float3 pos_)
{
	objectPtr_->transform_->scale_ = { 1.0f,1.0f,1.0f };
	//↓これも個別にいるかも(横にずらす値))
	SetPos(pos_);
}

void IFE::EnemyHp::ScaleCalc(int8_t& decHp_)
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

void IFE::EnemyHp::Draw()
{
}

void IFE::EnemyHp::Finalize()
{
}

void IFE::EnemyHp::SetPos(Float3 pos_)
{
	Vector3 eye;
	Vector3 pPos;
	//正面ベクトル
	Vector3 frontVec;
	//アクション
	if (IFE::ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>()->GetMode() == false) {
		eye = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetActionCamera()->transform_->eye_;
		pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
		frontVec = pPos - eye;
	}
	//ドローン
	else {
		eye = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetDroneCamera()->transform_->target_;
		pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos();
		frontVec = eye - pPos;
	}
	frontVec.Normalize();
	Vector3 tmp = { 0,1,0 };
	//右ベクトルの作成
	Vector3 rightVec = frontVec.Cross(tmp);
	rightVec.Normalize();
	transform_->position_ = { pos_.x,pos_.y + 2,pos_.z };
	transform_->position_ += (rightVec * 1.8f);
}
