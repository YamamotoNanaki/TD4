#include "enemyAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"
#include"Player.h"
#include "Sound.h"

void IFE::EnemyAttack::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->SetColor({ 0,0,1,1 });
	isShot = false;
	if (!objectPtr_->GetComponent<IFE::Collider>())
	{
		objectPtr_->AddComponent<IFE::Collider>();
		auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
		ptr->SetNoPushBackFlag(true);
		ptr->active_ = false;
	}

	player_ = IFE::ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>();
	IFE::Sound::Instance()->LoadWave("attackHit");
	IFE::Sound::Instance()->SetVolume("attackHit", 50);
}

void IFE::EnemyAttack::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	myCollider;
	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<PlayerAction>())
	{
		//当たった時の処理
		hitCollider->objectPtr_->GetComponent<PlayerAction>()->DecHp();

	}

	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<PlayerDrone>())
	{
		//当たった時の処理
		if (isShot == true) {
			player_->DroneBreak();
			objectPtr_->DrawFlag_ = false;
			isShot = false;

		}
	}
}

void IFE::EnemyAttack::Update()
{
}

void IFE::EnemyAttack::Finalize()
{
}

void IFE::EnemyAttack::ComponentDebugGUI()
{
}
