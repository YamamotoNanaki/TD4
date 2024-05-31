#include "EnemyBackColl.h"
#include "ObjectManager.h"
#include "Collider.h"

void IFE::EnemyBackColl::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->AddComponent<IFE::Collider>();
	auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
	ptr->SetNoPushBackFlag(true);
}

void IFE::EnemyBackColl::Update()
{
}
