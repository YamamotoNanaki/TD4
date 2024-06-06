#include "PlayerAttack.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Collider.h"
#include"NormalEnemy.h"

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
}

void PlayerAttack::Update()
{
	auto playerCol = objectPtr_->GetComponent<IFE::Collider>();
	playerCol->GetCollider(0)->active_ = isAttack_;
}

void PlayerAttack::Finalize()
{
}

void PlayerAttack::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	myCollider;
	if (isAttack_ == true && (hitCollider->GetColliderType() == IFE::ColliderType::SPHERE && hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()))
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetIsHit() == false)
		{
			if (hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->GetBack() == false)
			{
				//当たった時の処理
				hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->DecHp();
			}
			else
			{
				//当たった時の処理
				hitCollider->objectPtr_->GetComponent<IFE::NormalEnemy>()->OneShot();
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
