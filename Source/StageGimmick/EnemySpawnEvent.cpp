#include "EnemySpawnEvent.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "Transform.h"

EnemySpawnEvent::EnemySpawnEvent()
{
}

EnemySpawnEvent::~EnemySpawnEvent()
{
}


void EnemySpawnEvent::Initialize()
{
	


}

void EnemySpawnEvent::Update()
{
	if (!isEnd_)
	{
		//クールタイムが終わっているなら
		if (enemySpawnCT_ >= enemySpawnMaxCT_)
		{
			IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
			auto& list = objm->GetObjList();
			enemyNum_ = 0;

			for (auto& itr : list)
			{
				if (itr->GetObjectName().find("TrapEnemy") != std::string::npos)
				{
					enemyNum_++;
				}
				else if (itr->GetObjectName().find("EnemyManager") != std::string::npos)
				{
					EnemyManagerObject_ = itr.get();
				}
			}

			for (int32_t i = 0; i < enemySpawnNum_; i++)
			{
				if (enemyNum_ >= MaxEnemyNum_)
				{
					break;
				}
				EnemyManagerObject_->GetComponent<IFE::EnemyManager>()->PopEnemy(spawnPos_, { 0,0,0 }, trapPos_);
				enemyNum_++;

			}
			enemySpawnCT_ = 0;
			isEnd_ = true;
		}
		else
		{
			if (enemySpawnCT_ < enemySpawnMaxCT_)
			{
				enemySpawnCT_ += IFE::IFETime::sDeltaTime_;
			}
			else
			{
				isEnd_ = true;
			}
		}
	}
}
#ifdef EditorMode
#include "ImguiManager.h"
void EnemySpawnEvent::DebugGUI()
{
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloat3GUI(&spawnPos_, "spawnPos", 0.25f, -1000, 1000);
	gui->DragIntGUI(&enemySpawnNum_, "enemySpawnNum", 1.0f, 1, 100);
	gui->DragIntGUI(&MaxEnemyNum_, "MaxSpawnEnemyNum", 1.0f, 1, 100);
	gui->DragFloatGUI(&enemySpawnMaxCT_, "enemySpawnCT_", 1.0f, 1, 100);
}
#endif

void EnemySpawnEvent::OutputData(nlohmann::json& json)
{
	IFE::JsonManager::Instance()->OutputFloat3(json["enemySpawnPos"], spawnPos_);

	IFE::Float2 output = { (float)enemySpawnNum_, (float)MaxEnemyNum_ };
	IFE::JsonManager::Instance()->OutputFloat2(json["enemySpawnNum"], output);
}

void EnemySpawnEvent::InputData(nlohmann::json& json)
{

	spawnPos_.x = json["enemySpawnPos"][0];
	spawnPos_.y = json["enemySpawnPos"][1];
	spawnPos_.z = json["enemySpawnPos"][2];

	enemySpawnNum_ = (int32_t)json["enemySpawnNum"][0];
	MaxEnemyNum_ = (int32_t)json["enemySpawnNum"][1];

}

void EnemySpawnEvent::StartInitialize()
{
	isEnd_ = false;
}

void EnemySpawnEvent::InputTransform(const IFE::Transform& transform)
{
	trapPos_ = transform.position_;
}