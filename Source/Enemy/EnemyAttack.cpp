#include "enemyAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"
#include"Player.h"

void IFE::EnemyAttack::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->SetColor({ 0,0,1,1 });
	if (!objectPtr_->GetComponent<IFE::Collider>())
	{
		objectPtr_->AddComponent<IFE::Collider>();
		auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
		ptr->SetNoPushBackFlag(true);
		ptr->active_ = false;
	}

	player_ = IFE::ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>();
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
		player_->DroneBreak();
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
