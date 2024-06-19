#include "enemyAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"
#include "PlayerAction.h"
#include"PlayerDrone.h"

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
		//“–‚½‚Á‚½Žž‚Ìˆ—
		hitCollider->objectPtr_->GetComponent<PlayerAction>()->DecHp();
	}

	if (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<PlayerDrone>())
	{
		//“–‚½‚Á‚½Žž‚Ìˆ—
		hitCollider->objectPtr_->GetComponent<PlayerDrone>()->SetIsDroneSurvival(false);
		if (player_->GetMode() == true)
		{
			player_->SetMode(false);
			IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
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
