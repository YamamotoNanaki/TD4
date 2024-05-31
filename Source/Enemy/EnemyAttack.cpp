#include "enemyAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"

void IFE::EnemyAttack::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->SetColor({ 0,0,1,1 });
	objectPtr_->AddComponent<IFE::Collider>(); 
	auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
	ptr->SetNoPushBackFlag(true);
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
