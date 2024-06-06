#include "EnemyManager.h"
#include "Object3d.h"
#include "Transform.h"
//#include "JsonManager.h"
#include "ObjectManager.h"
#include "Collider.h"
//#include "ModelManager.h"

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
			col0->attribute_ = uint16_t(Attribute::ENEMYS);
			auto col1 = com->AddCollider();
			col1->SetColliderType(ColliderType::SPHERE);
			col1->attribute_ = uint16_t(Attribute::ENEMYS);
			col1->SetPushBackFlag(true);
			col1->SetGroundJudgeFlag(true);
			enemyList_.push_back(enemy);
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
	if (enemyList_.size() != 0)enemyList_.remove_if([](NormalEnemy* ne) {return ne->objectPtr_->GetDeleteFlag(); });
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
