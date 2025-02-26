#include "PlayerAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"
#include"NormalEnemy.h"
#include "TrapEnemy.h"
#include "Boss.h"
#include "Sound.h"

void PlayerAttack::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->SetColor({ 0,0,1,1 });
	if (!objectPtr_->GetComponent<IFE::Collider>())
	{
		objectPtr_->AddComponent<IFE::Collider>();
		auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
		ptr->SetNoPushBackFlag(true);
	}
	IFE::Sound::Instance()->LoadWave("attackHit", IFE::Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("attackHit", 30);
	IFE::Sound::Instance()->LoadWave("backstab", IFE::Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("backstab", 80);

}

void PlayerAttack::Update()
{
	/*auto playerCol = objectPtr_->GetComponent<IFE::Collider>();
	playerCol->GetCollider(0)->active_ = isAttack_;*/
}

void PlayerAttack::Finalize()
{
}

void PlayerAttack::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	myCollider;

	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>())
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetBack(0) == false)
		{
			isBackAttack_ = false;
		}
		else
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetHP() > 0) {
				isBackAttack_ = true;
				if (attackFlag_ && hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetIsOneShot() == false) {
					hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->SetisOneShot(true);
					hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->Killed();
				}
			}
			else {
				isBackAttack_ = false;
			}
		}

		if (isAttack_ == true)
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetIsHit() == false)
			{
				if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetBack(0) == false)
				{
					//当たった時の処理
					hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->DecHp();
					IFE::Sound::Instance()->SoundPlay("attackHit", false, true);
				}
				else
				{
					//当たった時の処理
					hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->OneShot();
					IFE::Sound::Instance()->SoundPlay("backstab", false, true);
				}
			}
		}
	}
	//trapEnemy
	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>())
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->GetBack(0) == false)
		{
			isBackAttack_ = false;
		}
		else
		{
			isBackAttack_ = true;
			if (attackFlag_ && hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->GetIsOneShot() == false) {
				hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->SetisOneShot(true);
				hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->Killed();

			}
		}

		if (isAttack_ == true)
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->GetIsHit() == false)
			{
				if (hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->GetBack(0) == false)
				{
					//当たった時の処理
					hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->DecHp();
					IFE::Sound::Instance()->SoundPlay("attackHit", false, true);
				}
				else
				{
					//当たった時の処理
					hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->OneShot();
					IFE::Sound::Instance()->SoundPlay("backstab", false, true);
				}
			}
		}
	}
	//boss
	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<IFE::Boss>())
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::Boss>()->GetBack(0) == false)
		{
			isBackAttack_ = false;
		}
		else
		{
			isBackAttack_ = true;
			if (attackFlag_ && hitCollider->objectPtr_->GetComponent<IFE::Boss>()->GetIsOneShot() == false) {
				hitCollider->objectPtr_->GetComponent<IFE::Boss>()->SetisOneShot(true);
				hitCollider->objectPtr_->GetComponent<IFE::Boss>()->Killed();

			}
		}

		if (isAttack_ == true)
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::Boss>()->GetIsHit() == false)
			{
				if (hitCollider->objectPtr_->GetComponent<IFE::Boss>()->GetBack(0) == false)
				{
					//当たった時の処理
					hitCollider->objectPtr_->GetComponent<IFE::Boss>()->DecHp();
					IFE::Sound::Instance()->SoundPlay("attackHit", false, true);
				}
				else
				{
					//当たった時の処理
					hitCollider->objectPtr_->GetComponent<IFE::Boss>()->OneShot();
					IFE::Sound::Instance()->SoundPlay("backstab", false, true);
				}
			}
		}
	}
}

void PlayerAttack::SetName(std::string name)
{
	name_ = name;
}

std::string PlayerAttack::GetName()
{
	return name_;
}

void PlayerAttack::SetIsAttack(bool flag)
{
	isAttack_ = flag;
}

const bool PlayerAttack::GetIsAttack()
{
	return isAttack_;
}

bool PlayerAttack::GetIsBackAttack()
{
	return isBackAttack_;
}

void PlayerAttack::SetIsBackAttack(bool flag)
{
	isBackAttack_ = flag;
}

void PlayerAttack::SetAttackFlag(bool atk) {
	attackFlag_ = atk;
}