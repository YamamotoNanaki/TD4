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

void IFE::BaseEnemy::SetSound()
{
	IFE::Sound::Instance()->LoadWave("attack");
	IFE::Sound::Instance()->SetVolume("attack", 60);
	IFE::Sound::Instance()->LoadWave("gun");
	IFE::Sound::Instance()->SetVolume("gun", 60);
	//IFE::Sound::Instance()->LoadMP3("what");
	//IFE::Sound::Instance()->SetVolume("what", 60);
	//IFE::Sound::Instance()->LoadMP3("found");
	//IFE::Sound::Instance()->SetVolume("found", 60);
	/*IFE::Sound::Instance()->LoadWave("walk");
	IFE::Sound::Instance()->SetVolume("walk", 4);*/
}

void IFE::BaseEnemy::DecHp()
{
	if (isHit_ == false) {
		if (hp_ > 0) {
			ani_->SetAnimation("damage");
		}
		else if(hp_ <= 0) {
			ani_->SetAnimation("downBack");
		}
		hp_ -= 25;
		decHp_ = 25;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::BaseEnemy::OneShot()
{
	if (isHit_ == false) {
		ani_->SetAnimation("downFront");
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
