#include "MapUI.h"
#include "ObjectManager.h"
#include "SpriteManager.h"
#include "CameraManager.h"
#include "Transform.h"
#include "ColorBuffer.h"
#include "PlayerAction.h"
#include <unordered_set>
#include <algorithm>

MapUI::MapUI()
{
}

MapUI::~MapUI()
{
}

void MapUI::Initialize()
{
	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();
	spritePtr_->drawFlag_ = false;
	
	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("PlayerAction") != std::string::npos)
		{
			playerObj_ = itr.get();
		}
		
	}
}

static void SpriteEnemyDelete(const std::vector<std::pair<IFE::Sprite*,std::string>>& vec, const std::list<IFE::BaseEnemy*>& lst)
{
	std::unordered_set<std::string> setLst;
	for (const auto& item : lst)
	{
		setLst.insert(item->objectPtr_->GetObjectName());
	}

	std::vector<IFE::Sprite*> deleteElements;
	for (const auto& item : vec)
	{
		if (setLst.find(item.second) == setLst.end())
		{
			deleteElements.push_back(item.first);
		}
	}

	for (size_t i = 0; i < deleteElements.size(); i++)deleteElements[i]->Destroy();
}

void MapUI::Update()
{
	if (!IsInit_)
	{
		StartInit();
	}

	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();

	SpriteEnemyDelete(enemySprite_, objm->GetObjectPtr("EnemyManager")->GetComponent<IFE::EnemyManager>()->GetEnemyList());

	uint8_t count = 0;
	uint8_t enemyCount = 0;

	IFE::Vector3 frontVec = IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->target_ - IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->eye_;
	frontVec.Normalize();
	//カメラのangleYの取得(正面ベクトルから取得)
	float angle = -(atan2(frontVec.z, frontVec.x) + (float)IFE::PI / 2);

	playerSprite_->transform_->position2D_ = { transform2D_->position2D_.x ,transform2D_->position2D_.y };
	playerSprite_->transform_->scale2D_ = { 0.1f ,0.1f };

	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("ground") != std::string::npos)
		{
			//自機と敵との位置の差
			IFE::Float2 difference = { playerObj_->transform_->position_.x - itr->objectPtr_->transform_->position_.x,
				itr->objectPtr_->transform_->position_.z - playerObj_->transform_->position_.z };
			//自機と敵との距離の算出
			float length = sqrt(difference.x * difference.x + difference.y * difference.y);

			//mapSprite_[count]->drawFlag_ = false;

			mapSprite_[count]->transform_->position2D_ = { transform2D_->position2D_.x + difference.x * cosf(angle) + difference.y * sinf(angle),transform2D_->position2D_.y + difference.y * cosf(angle) - difference.x * sinf(angle) };
			mapSprite_[count]->transform_->rotation2D_ = /*itr->objectPtr_->transform_->rotation_.y - */(float)IFE::PI / 2 - angle;
			mapSprite_[count]->transform_->scale2D_ = { itr->objectPtr_->transform_->scale_.x/100,itr->objectPtr_->transform_->scale_.z / 100 };

			//敵が出現しているかつレーダーの内側の範囲内にいるなら
			if (transform2D_->scale2D_.x >= length && transform2D_->scale2D_.y >= length)
			{
				mapSprite_[count]->drawFlag_ = true;
			}
			count++;
		}
		else if (itr->GetObjectName().find("normalEnemy") != std::string::npos)
		{
			//自機と敵との位置の差
			IFE::Vector2 difference = { playerObj_->transform_->position_.x - itr->objectPtr_->transform_->position_.x,
				itr->objectPtr_->transform_->position_.z - playerObj_->transform_->position_.z };

			//自機と敵との距離の算出
			float length = sqrt(difference.x * difference.x+ difference.y * difference.y);

			//mapSprite_[count]->drawFlag_ = false;

			enemySprite_[enemyCount].first->transform_->position2D_ = { transform2D_->position2D_.x + difference.x * cosf(angle) + difference.y * sinf(angle),transform2D_->position2D_.y + difference.y * cosf(angle) - difference.x * sinf(angle) };
			enemySprite_[enemyCount].first->transform_->rotation2D_ = (itr->objectPtr_->transform_->rotation_.z - (float)IFE::PI / 2 - angle);
			enemySprite_[enemyCount].first->transform_->scale2D_ = { itr->objectPtr_->transform_->scale_.x / 20,itr->objectPtr_->transform_->scale_.z / 20 };

			//敵が出現しているかつレーダーの内側の範囲内にいるなら
			if (transform2D_->scale2D_.x >= length && transform2D_->scale2D_.y >= length)
			{
				enemySprite_[enemyCount].first->drawFlag_ = true;
			}
			enemyCount++;
		}
		
	}
	
	
}

void MapUI::Draw()
{
	
}

void MapUI::Finalize()
{

}

void MapUI::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	
	myCollider;
	hitCollider;
}

#ifdef EditorMode
#include "ImguiManager.h"
void MapUI::ComponentDebugGUI()
{
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloat3GUI(&playerObj_->transform_->position_, "points", 0.25f, -1000, 1000);
}

void MapUI::OutputComponent(nlohmann::json& json)
{
	json;
}

#endif

void MapUI::LoadingComponent(nlohmann::json& json)
{
	json;
}

void MapUI::StartInit()
{
	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();
	uint8_t count = 0;
	uint8_t enemycount = 0;

	for (auto& itr : list)
	{
		if (itr->GetObjectName() == "PlayerAction")
		{
			playerSprite_ = IFE::SpriteManager::Instance()->AddInitialize(std::string("mapPlayre"), "white");
			playerSprite_->GetComponent<IFE::ColorBuffer>()->SetColor({ 0,0,1,1 });
		}
		else if (itr->GetObjectName().find("ground") != std::string::npos)
		{

			IFE::Sprite* add = IFE::SpriteManager::Instance()->AddInitialize(std::string("map" + std::to_string(count)), "white");

			mapSprite_.push_back(add);
			count++;
		}
		else if (itr->GetObjectName().find("normalEnemy") != std::string::npos)
		{

			IFE::Sprite* add = IFE::SpriteManager::Instance()->AddInitialize(std::string("mapEnemy" + std::to_string(enemycount)), "white");
			add->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,0,0,1 });
			enemySprite_.push_back(std::make_pair(add, itr->GetObjectName()));
			enemycount++;
		}
	}

	IsInit_ = true;
}