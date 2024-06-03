#include "EnemyBackColl.h"
#include "ObjectManager.h"
#include "Collider.h"

void IFE::EnemyBackColl::Initialize()
{
	/*objectPtr_->DrawFlag_ = false;*/
	objectPtr_->AddComponent<IFE::Collider>();
	objectPtr_->SetColor({ 0,1,0,1 });
	auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
	ptr->SetNoPushBackFlag(true);
}

void IFE::EnemyBackColl::Update()
{
}

void IFE::EnemyBackColl::Finalize()
{
}

void IFE::EnemyBackColl::ComponentDebugGUI()
{
}
