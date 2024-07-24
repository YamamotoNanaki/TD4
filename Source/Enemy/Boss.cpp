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
	//���S
	if (hpUI->GetIsDead() == true) {
		hpUI->objectPtr_->Destroy();
		objectPtr_->Destroy();
		/////-----�����ɃV�[���`�F���W--------//////
		Scene::Instance()->SetNextScene("GAMECLEAR");
	}
}

bool IFE::Boss::GetBack()
{
	Vector3 pFront = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetFrontVec();
	float result = pFront.Dot({ 0,0,1 });
	//+�Ȃ���
	if (result > 0) {
		return true;
	}
	return false;
}

void IFE::Boss::Killed() {
	Vector3 pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	Vector3 addVec = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetFrontVec();
	Vector3 rot = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetRot();
	transform_->position_ = pPos + addVec;
	transform_->rotation_ = rot;
	ani_->SetAnimation("standBy");
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
