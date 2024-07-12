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
	IFE::Sound::Instance()->SetVolume("attack", 50);
	IFE::Sound::Instance()->LoadWave("gun");
	IFE::Sound::Instance()->SetVolume("gun", 70);
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

void IFE::BaseEnemy::DecHp()
{
	ani_->SetAnimation("damage");
	if (isHit_ == false) {
		IFE::Sound::Instance()->SoundPlay("attackHit", false, true);
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
		IFE::Sound::Instance()->SoundPlay("backstab", false, true);
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
