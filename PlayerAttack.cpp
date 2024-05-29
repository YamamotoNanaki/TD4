#include "PlayerAttack.h"
#include "ObjectManager.h"
#include "Transform.h"

void PlayerAttack::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	objectPtr_->SetColor({ 0,0,1,1 });
}

void PlayerAttack::Update()
{
}

void PlayerAttack::Finalize()
{
}
