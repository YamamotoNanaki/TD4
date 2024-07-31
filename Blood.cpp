#include "Blood.h"
#include "IFEEffekseerManager.h"
#include "Input.h"
#include "Object3D.h"
#include "ObjectManager.h"
#include "PlayerAction.h"
#include "BaseEnemy.h"
#include "Attach3DModel.h"

using namespace IFE;

void Blood::Initialize()
{
	efk_ = IFEEffekseerManager::Instance()->GetEffekseer("blood");
	isAttack_ = ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetIsAttackPtr();
}

void Blood::Update()
{
	pos = transform_->GetWorldPosition();
	rot = transform_->GetWorldRotation();
	if (start_&&isAttack_)
	{
		efk_->Play(pos, rot, { 0.25,0.25,0.25 });
		start_ = false;
	}
}

void Blood::Play(BaseEnemy* enemy)
{
	objectPtr_->GetComponent<Attach3DModel>()->SetParent(enemy->objectPtr_);
	start_ = true;
}
