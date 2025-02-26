#include "EnemyManager.h"
#include "TrapEnemy.h"
#include "Object3d.h"
#include "Transform.h"
//#include "JsonManager.h"
#include "ObjectManager.h"
#include "Collider.h"
#include "Animator.h"
#include "ModelManager.h"
#include "Boss.h"

void IFE::EnemyManager::Initialize()
{
	if (enemyList_.size() != 0)return;
	ObjectManager* objM = ObjectManager::Instance();
	for (auto& itr : objM->GetObjList())
	{
		auto enemy = itr->GetComponent<NormalEnemy>();
		if (enemy)
		{
			enemy->objectPtr_->AddComponentBack<Collider>();
			auto com = enemy->objectPtr_->GetComponent<Collider>();
			auto col0 = com->AddCollider();
			col0->SetColliderType(ColliderType::RAY);

			auto col1 = com->AddCollider();
			col1->SetColliderType(ColliderType::SPHERE);
			col1->attribute_ = uint16_t(Attribute::ENEMYS);
			col1->SetPushBackFlag(true);
			col1->SetGroundJudgeFlag(true);

			auto col2 = com->AddCollider();
			col2->SetColliderType(ColliderType::RAY);


			enemyList_.push_back(enemy);
			itr->SetModel(IFE::ModelManager::Instance()->GetModel("enemyMotion"));
			itr->AddComponent<IFE::Animator>();

			auto anim = itr->GetComponent<IFE::Animator>();
			anim->SetAnimation("standBy", false);
			anim->loop_ = true;
			continue;
		}
		auto boss = itr->GetComponent<IFE::Boss>();
		if (boss)
		{
			boss->objectPtr_->AddComponentBack<Collider>();
			auto com = boss->objectPtr_->GetComponent<Collider>();
			auto col0 = com->AddCollider();

			col0->SetColliderType(ColliderType::RAY);

			auto col1 = com->AddCollider();
			col1->SetColliderType(ColliderType::SPHERE);
			col1->attribute_ = uint16_t(Attribute::ENEMYS);

			col1->SetPushBackFlag(true);
			col1->SetGroundJudgeFlag(true);

			auto col2 = com->AddCollider();
			col2->SetColliderType(ColliderType::RAY);

			enemyList_.push_back(boss);
			itr->SetModel(IFE::ModelManager::Instance()->GetModel("enemyMotion"));

			itr->AddComponent<IFE::Animator>();
			auto anim = itr->GetComponent<IFE::Animator>();
			anim->SetAnimation("walk", false);
			anim->loop_ = true;
		}
	}
	//ModelManager* modelM = ModelManager::Instance();
	//for (size_t i = 0; i < enemyPositions_.size(); i++)
	//{
	//	/*auto enemy = */objM->AddInitialize("NormalEnemy", modelM->GetModel("normalEnemyModel"));
	//	//enemyList_.push_back(enemy);
	//}
}

void IFE::EnemyManager::Update()
{
	objectPtr_->DrawFlag_ = false;
	if (enemyList_.size() != 0)enemyList_.remove_if([](BaseEnemy* ne) {return ne->GetIsDead(); });
}

/// <summary>
/// とりあえず出すだけ(まだ徘徊はしない)
/// </summary>
/// <param name="position"></param>
/// <param name="rotation"></param>
void IFE::EnemyManager::PopEnemy(Vector3 position, Vector3 rotation, Vector3 trapPos_)
{
	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("TrapEnemy", ModelManager::Instance()->GetModel("enemyMotion"));
	ptr->AddComponent<TrapEnemy>();
	TrapEnemy* enemy = ptr->GetComponent<TrapEnemy>();
	enemy->objectPtr_->transform_->position_ = position;
	enemy->objectPtr_->transform_->rotation_ = rotation;
	enemy->SetTrapPos(trapPos_);
	enemy->objectPtr_->AddComponentBack<Collider>();
	auto com = enemy->objectPtr_->GetComponent<Collider>();
	auto col0 = com->AddCollider();
	col0->SetColliderType(ColliderType::RAY);

	auto col1 = com->AddCollider();
	col1->SetColliderType(ColliderType::SPHERE);
	col1->attribute_ = uint16_t(Attribute::ENEMYS);
	col1->SetPushBackFlag(true);
	col1->SetGroundJudgeFlag(true);

	auto col2 = com->AddCollider();
	col2->SetColliderType(ColliderType::RAY);
	/*ptr->AddComponent<IFE::Animator>();*/

	enemy->objectPtr_->AddComponentBack<IFE::Animator>();

	auto anim = enemy->objectPtr_->GetComponent<IFE::Animator>();
	anim->SetAnimation("walk", false);
	anim->loop_ = true;

	enemyList_.push_back(enemy);
}

#ifdef EditorMode
#include "ImguiManager.h"

void IFE::EnemyManager::ComponentDebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();
	std::string text = "enemy num : ";
	text += std::to_string(enemyList_.size());
	gui->TextGUI(text);
	//gui->DragVectorFloat3GUI(enemyPositions_, "enemy position", transform_->position_);
}

void IFE::EnemyManager::OutputComponent(nlohmann::json& json)
{
	//JsonManager* j = JsonManager::Instance();
	//for (size_t i = 0; i < enemyPositions_.size(); i++)
	//{
	//	j->OutputFloat3(json["enemyPos"][i], enemyPositions_[i]);
	//}
	json;
}

#endif
void IFE::EnemyManager::LoadingComponent(nlohmann::json& json)
{
	//JsonManager* j = JsonManager::Instance();
	//for (auto& itr : json["enemyPos"])
	//{
	//	enemyPositions_.push_back(j->InputFloat3(itr));
	//}
	json;
}
