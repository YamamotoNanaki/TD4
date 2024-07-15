#include "BaseEnemy.h"
#include "Collider.h"
#include "Object3d.h"
#include "transform.h"
#include "ObjectManager.h"
#include "PlayerDrone.h"
#include "IFETime.h"
#include "Sound.h"

using namespace IFE;

IFE::Vector2 IFE::BaseEnemy::GetPos()
{
	return { transform_->position_.x, transform_->position_.z };
}

void IFE::BaseEnemy::Highlighting()
{
	Vector3 dir = ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->transform_->position_ - transform_->position_;
	dir.Normalize();
	objectPtr_->GetComponent<Collider>()->GetCollider(0)->rayDir_ = dir;

	droneHit_ = false;
	droneHitDistance_ = 0;
	if (hitObject_)
	{
		if (hitObject_->GetComponent<PlayerDrone>())
		{
			droneHit_ = true;
			droneHitDistance_ = hitDistance_;
		}
	}

	hitObject_ = nullptr;
	hitDistance_ = 0;

	ani_ = objectPtr_->GetComponent<IFE::Animator>();

	IFE::Sound::Instance()->LoadWave("attack");
	IFE::Sound::Instance()->SetVolume("attack", 25);
	IFE::Sound::Instance()->LoadWave("gun");
	IFE::Sound::Instance()->SetVolume("gun", 50);
}

void IFE::BaseEnemy::Update()
{
	//hitcool
	if (isHit_ == true) {
		hitTime_-= IFE::IFETime::sDeltaTime_ * 3.5f;
		if (hitTime_ <= 0) {
			isHit_ = false;
		}
	}
	EnemyUpdate();
	Highlighting();
}

void IFE::BaseEnemy::ApproachTarget(float& current, float target, float step)
{
	if (std::abs(current - target) < step) {
		current = target; // 目標値にほぼ達した場合、目標値に設定
	}
	else if (current < target) {
		current += step; // 現在値が目標値より小さい場合、増加
	}
	else {
		current -= step; // 現在値が目標値より大きい場合、減少
	}
}

void IFE::BaseEnemy::DecHp()
{
	ani_->SetAnimation("damage");
	if (isHit_ == false) {
		hp_ -= 25;
		decHp_ = 25;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::BaseEnemy::OneShot()
{
	ani_->SetAnimation("downFront");
	if (isHit_ == false) {
		hp_ -= hp_;
		decHp_ = hp_;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::BaseEnemy::OnColliderHit(ColliderCore* mycol, ColliderCore* hitcol)
{
	if (mycol->colliderNumber_ == 0)
	{
		if (mycol->rayDistance > -2)
		{
			if (hitcol->objectPtr_)
			{
				if (!hitObject_)
				{
					hitObject_ = hitcol->objectPtr_;
					hitDistance_ = mycol->rayDistance;
				}
				else if (hitObject_->GetObjectName() != "PlayerDrone");
				else
				{
					if (hitDistance_ > mycol->rayDistance)
					{
						hitObject_ = hitcol->objectPtr_;
						hitDistance_ = mycol->rayDistance;
					}
				}
			}
		}
	}
	EnemyOnColliderHit(mycol, hitcol);
}
