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
	if (start_ && isAttack_)
	{
		auto r = ConvertToDegrees(transform_->GetWorldRotation());
		r.z = 0;
		efk_->Play(transform_->GetWorldPosition(), r, { 0.25,0.25,0.25 });
		start_ = false;
	}
}

void Blood::Play(BaseEnemy* enemy)
{
	objectPtr_->GetComponent<Attach3DModel>()->SetParent(enemy->objectPtr_);
	start_ = true;
}

#ifdef EditorMode
#include "ImguiManager.h"
void Blood::ComponentDebugGUI()
{
	ImguiManager::Instance()->TextFloat3GUI(ConvertToDegrees(transform_->GetWorldRotation()));
}
#endif
