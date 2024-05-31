#include "PlayerAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"

void PlayerAttack::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->SetColor({ 0,0,1,1 });
	objectPtr_->AddComponent<IFE::Collider>();
	auto ptr = objectPtr_->GetComponent<IFE::Collider>()->AddCollider();
	ptr->SetNoPushBackFlag(true);
}

void PlayerAttack::Update()
{

}

void PlayerAttack::Finalize()
{
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
