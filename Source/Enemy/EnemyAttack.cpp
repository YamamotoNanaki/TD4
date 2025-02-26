#include "enemyAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"
#include"Player.h"
#include "Sound.h"

void IFE::EnemyAttack::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->SetColor({ 0,0,0,1 });
	isShot = false;
	if (!objectPtr_->GetComponent<IFE::Collider>())
	{
		objectPtr_->AddComponent<IFE::Collider>();
		auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
		ptr->SetNoPushBackFlag(true);
		ptr->active_ = false;
	}

	player_ = IFE::ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>();
	IFE::Sound::Instance()->LoadWave("attackHit", Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("attackHit", 30);
	IFE::Sound::Instance()->LoadMP3("droneBreak", Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("droneBreak", 40);
	IFE::Sound::Instance()->LoadMP3("droneShock", Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("droneShock", 50);
}

void IFE::EnemyAttack::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	myCollider;
	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<PlayerAction>())
	{
		//当たった時の処理
		hitCollider->objectPtr_->GetComponent<PlayerAction>()->DecHp(isBack);
		if (IFE::Sound::Instance()->GetPlayStatus("attackHit") == false) {
			transform_->position_ = { 0, -10, 0 };
			IFE::Sound::Instance()->SoundPlay("attackHit", false, true);
		}

	}

	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<PlayerDrone>())
	{
		//当たった時の処理
		if (isShot == true) {
			IFE::Sound::Instance()->SoundPlay("droneBreak", false, true);
			IFE::Sound::Instance()->SoundPlay("droneShock", false, true);
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
