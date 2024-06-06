#include "Boss.h"
#include "Transform.h"
#include "Collider.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "StageCollideManageer.h"
#include "TextureManager.h"
#include "PlayerAction.h"
#include "Scene.h"

void IFE::Boss::Initialize()
{
	state = SEARCH;
	preState = state;
	hp_ = 100;
	decHp_ = 0;
	isHit_ = false;
	hitTime_ = 0;
	objectPtr_->SetColor({ 0, 1, 1, 1 });
	//HPUI
	if (!hpUI)
	{
		auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyHp", ModelManager::Instance()->GetModel("hppanel"));
		ptr->AddComponent<EnemyHp>();
		hpUI = ptr->GetComponent<EnemyHp>();
	}
}

void IFE::Boss::EnemyUpdate()
{
	hpUI->Update(transform_->position_,hp_,decHp_);
	//死亡
	if (hpUI->GetIsDead() == true) {
		hpUI->objectPtr_->Destroy();
		objectPtr_->Destroy();
		/////-----ここにシーンチェンジ--------//////
		Scene::Instance()->SetNextScene("GAMECLEAR");
	}
}

bool IFE::Boss::GetBack()
{
	Vector3 pFront = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetFrontVec();
	float result = pFront.Dot({ 0,0,1 });
	//+なら後ろ
	if (result > 0) {
		return true;
	}
	return false;
}

#ifdef EditorMode
void IFE::Boss::ComponentDebugGUI()
{
}
#endif

void IFE::Boss::EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	myCollider;
	hitCollider;
}

void IFE::Boss::Draw()
{
}

void IFE::Boss::Finalize()
{
	delete hpUI;
}
