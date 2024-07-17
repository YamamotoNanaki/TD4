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
	IFE::Sound::Instance()->LoadWave("attackHit");
	IFE::Sound::Instance()->SetVolume("attackHit", 20);
	IFE::Sound::Instance()->LoadWave("backstab");
	IFE::Sound::Instance()->SetVolume("backstab", 50);

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
		if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetBack() == false)
		{
			isBackAttack_ = false;
		}
		else
		{
			isBackAttack_ = true;
		}

		if (isAttack_ == true)
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetIsHit() == false)
			{
				if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetBack() == false)
				{
					//“–‚½‚Á‚½Žž‚Ìˆ—
					hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->DecHp();
					IFE::Sound::Instance()->SoundPlay("attackHit", false, true);
				}
				else
				{
					//“–‚½‚Á‚½Žž‚Ìˆ—
					hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->OneShot();
					IFE::Sound::Instance()->SoundPlay("backstab", false, true);
				}
			}
		}
	}
	//trapEnemy
	if (isAttack_ == true && (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()))
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->GetIsHit() == false)
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->GetBack() == false)
			{
				//“–‚½‚Á‚½Žž‚Ìˆ—
				hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->DecHp();
			}
			else
			{
				//“–‚½‚Á‚½Žž‚Ìˆ—
				hitCollider->objectPtr_->GetComponent<IFE::TrapEnemy>()->OneShot();
			}
		}
	}
	//boss
	if (isAttack_ == true && (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<IFE::Boss>()))
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::Boss>()->GetIsHit() == false)
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::Boss>()->GetBack() == false)
			{
				//“–‚½‚Á‚½Žž‚Ìˆ—
				hitCollider->objectPtr_->GetComponent<IFE::Boss>()->DecHp();
				IFE::Sound::Instance()->SoundPlay("attack", false, true);
			}
			else
			{
				//“–‚½‚Á‚½Žž‚Ìˆ—
				hitCollider->objectPtr_->GetComponent<IFE::Boss>()->OneShot();
				IFE::Sound::Instance()->SoundPlay("backstab", false, true);
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