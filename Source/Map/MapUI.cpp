#include "MapUI.h"
#include "ObjectManager.h"
#include "SpriteManager.h"
#include "CameraManager.h"
#include "Transform.h"
#include "ColorBuffer.h"
#include "PlayerAction.h"
#include "Player.h"
#include "PlayerDrone.h"
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
#ifdef EditorMode
	spritePtr_->GetComponent<IFE::ColorBuffer>()->SetColor({ 1,1,1,0.2f });
#else
	spritePtr_->drawFlag_ = false;
#endif
	

	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("playerObject") != std::string::npos)
		{
			playerObj_ = itr.get();
		}
		else if (itr->GetObjectName().find("PlayerAction") != std::string::npos)
		{
			playerActionObj_ = itr.get();
		}
		else if (itr->GetObjectName().find("PlayerDrone") != std::string::npos)
		{
			playerDroneObj_ = itr.get();
		}

	}
}

static void SpriteEnemyDelete(std::vector<std::pair<IFE::Sprite*, std::string>>& vec, const std::list<IFE::BaseEnemy*>& lst)
{
	std::unordered_set<std::string> setLst;
	for (const auto& item : lst)
	{
		setLst.insert(item->objectPtr_->GetObjectName());
	}

	std::vector<std::pair<IFE::Sprite*, std::string>> comonElements;
	for (const auto& item : vec)
	{
		if (setLst.find(item.second) != setLst.end())
		{
			comonElements.push_back(item);
		}
		else
		{
			item.first->Destroy();
		}
	}
	std::swap(vec, comonElements);
}

void MapUI::Update()
{
	if (!IsInit_)
	{
		StartInit();
	}

	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();

	SpriteEnemyDelete(enemySprite_, objm->GetObjectPtr("EnemyManager")->GetComponent<IFE::EnemyManager>()->GetEnemyList());

	playerSprite_->transform_->position2D_ = { transform2D_->position2D_.x ,transform2D_->position2D_.y };
	playerSprite_->transform_->scale2D_ = { 0.1f ,0.1f };
	playerSprite_->order_ = 100;

	if (isMapRot_)
	{
		RotationMapUpdate();
	}
	else
	{
		FixedMapUpdate();
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
	gui->CheckBoxGUI(&isMapRot_, "mapRot");
	if (!playerObj_)return;
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


void MapUI::RotationMapUpdate()
{
	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();

	uint8_t count = 0;
	uint8_t enemyCount = 0;
	IFE::Float2 pos;

	IFE::Vector3 frontVec;

	if (playerObj_->objectPtr_->GetComponent<Player>()->GetMode())
	{
		pos = { playerDroneObj_->transform_->position_.x ,playerDroneObj_->transform_->position_.z };
		frontVec = IFE::CameraManager::Instance()->GetCamera("DroneCamera")->transform_->target_ - IFE::CameraManager::Instance()->GetCamera("DroneCamera")->transform_->eye_;
	}
	else
	{
		pos = { playerActionObj_->transform_->position_.x ,playerActionObj_->transform_->position_.z };
		frontVec = IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->target_ - IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->eye_;
	}

	frontVec.Normalize();
	//カメラのangleYの取得(正面ベクトルから取得)
	float angle = -(atan2(frontVec.z, frontVec.x) + (float)IFE::PI / 2);
	float degrees = IFE::ConvertToDegrees(-angle);

	

	

	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("ground") != std::string::npos)
		{
			//自機と敵との位置の差
			IFE::Float2 difference = { pos.x - itr->objectPtr_->transform_->position_.x,
				itr->objectPtr_->transform_->position_.z - pos.y };
			//自機と敵との距離の算出
			float length = sqrt(difference.x * difference.x + difference.y * difference.y);

			mapSprite_[count]->transform_->position2D_ = { transform2D_->position2D_.x + difference.x * cosf(angle) + difference.y * sinf(angle),transform2D_->position2D_.y + difference.y * cosf(angle) - difference.x * sinf(angle) };
			mapSprite_[count]->transform_->rotation2D_ = itr->objectPtr_->transform_->rotation_.y + degrees;
			mapSprite_[count]->transform_->scale2D_ = { itr->objectPtr_->transform_->scale_.x / 50,itr->objectPtr_->transform_->scale_.z / 50 };

			float alpha = 0;

			IFE::Float2 MapScale = { transform2D_->scale2D_.x * 50,transform2D_->scale2D_.y * 50 };
			
			//範囲外にいる場合(固定のほうと大きさの部分が違うが同じにしたらチカチカして目に悪くこっちにしたら安定したため)
			if (!OBB(IFE::SetVector2(transform2D_->position2D_), MapScale, IFE::SetVector2(mapSprite_[count]->transform_->position2D_), mapSprite_[count]->transform_->scale2D_, playerSprite_->transform_->rotation2D_, mapSprite_[count]->transform_->rotation2D_))
			{
				
				IFE::Float2 overLength = { length - transform2D_->scale2D_.x,length - transform2D_->scale2D_.y };
				float maxOverLength;
				//距離が離れている方を取る
				if (overLength.x > overLength.y)
				{
					maxOverLength = overLength.x;
				}
				else
				{
					maxOverLength = overLength.y;
				}

				alpha = (150 - maxOverLength) / 255;

			}
			else
			{
				alpha = 1;
			}

			mapSprite_[count]->GetComponent<IFE::ColorBuffer>()->SetAlpha(alpha);

			mapSprite_[count]->order_ = 0;
			count++;
		}
		else if (itr->GetObjectName().find("normalEnemy") != std::string::npos)
		{
			//自機と敵との位置の差
			IFE::Vector2 difference = { pos.x - itr->objectPtr_->transform_->position_.x,
				itr->objectPtr_->transform_->position_.z - pos.y };

			//自機と敵との距離の算出
			float length = sqrt(difference.x * difference.x + difference.y * difference.y);

			enemySprite_[enemyCount].first->drawFlag_ = false;

			enemySprite_[enemyCount].first->transform_->position2D_ = { transform2D_->position2D_.x + difference.x * cosf(angle) + difference.y * sinf(angle),transform2D_->position2D_.y + difference.y * cosf(angle) - difference.x * sinf(angle) };
			enemySprite_[enemyCount].first->transform_->rotation2D_ = (itr->objectPtr_->transform_->rotation_.y + degrees);
			enemySprite_[enemyCount].first->transform_->scale2D_ = { itr->objectPtr_->transform_->scale_.x / 20,itr->objectPtr_->transform_->scale_.z / 20 };

			//一定距離内にいるなら
			if (20 >= length)
			{
				enemySprite_[enemyCount].first->drawFlag_ = true;

			}
			enemySprite_[enemyCount].first->order_ = 1;
			enemyCount++;
		}

	}
}

void MapUI::FixedMapUpdate()
{
	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();

	uint8_t count = 0;
	uint8_t enemyCount = 0;

	//IFE::Vector3 frontVec = IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->target_ - IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->eye_;
	//frontVec.Normalize();
	////カメラのangleYの取得(正面ベクトルから取得)
	//float angle = -(atan2(frontVec.z, frontVec.x) + (float)IFE::PI / 2);

	IFE::Float2 pos;

	if (playerObj_->objectPtr_->GetComponent<Player>()->GetMode())
	{
		pos = { playerDroneObj_->transform_->position_.x ,playerDroneObj_->transform_->position_.z };
	}
	else
	{
		pos = { playerActionObj_->transform_->position_.x ,playerActionObj_->transform_->position_.z };
	}

	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("ground") != std::string::npos)
		{
			//自機と敵との位置の差
			IFE::Float2 difference = { pos.x - itr->objectPtr_->transform_->position_.x,
				itr->objectPtr_->transform_->position_.z - pos.y };
			//自機と敵との距離の算出
			float length = sqrt(difference.x * difference.x + difference.y * difference.y);

			//mapSprite_[count]->drawFlag_ = false;

			mapSprite_[count]->transform_->position2D_ = { transform2D_->position2D_.x + difference.x,transform2D_->position2D_.y + difference.y};
			mapSprite_[count]->transform_->rotation2D_ = 0;
			mapSprite_[count]->transform_->scale2D_ = { itr->objectPtr_->transform_->scale_.x / 50,itr->objectPtr_->transform_->scale_.z / 50 };

			IFE::Float2 MapScale = { transform2D_->scale2D_.x * 50,transform2D_->scale2D_.y * 50 };

			float alpha=0;
			//範囲外にいる場合(スプライトのほうがオブジェクトの大きさを渡しているのは判定上真ん中じゃないと反応しなかったから)
			if (!AABB(transform2D_->position2D_, MapScale, mapSprite_[count]->transform_->position2D_, { itr->objectPtr_->transform_->scale_.x ,itr->objectPtr_->transform_->scale_.z}))
			{

				IFE::Float2 overLength = { length - transform2D_->scale2D_.x,length - transform2D_->scale2D_.y };
				float maxOverLength;
				//距離が離れている方を取る
				if (overLength.x > overLength.y)
				{
					maxOverLength = overLength.x;
				}
				else
				{
					maxOverLength = overLength.y;
				}

				alpha = (100 - maxOverLength) / 255;

			}
			else
			{
				alpha = 1;
			}

			mapSprite_[count]->GetComponent<IFE::ColorBuffer>()->SetAlpha(alpha);

			mapSprite_[count]->order_ = 0;
			count++;
		}
		else if (itr->GetObjectName().find("normalEnemy") != std::string::npos)
		{
			//自機と敵との位置の差
			IFE::Vector2 difference = { pos.x - itr->objectPtr_->transform_->position_.x,
				itr->objectPtr_->transform_->position_.z - pos.y };

			//自機と敵との距離の算出
			float length = sqrt(difference.x * difference.x + difference.y * difference.y);

			enemySprite_[enemyCount].first->drawFlag_ = false;

			enemySprite_[enemyCount].first->transform_->position2D_ = { transform2D_->position2D_.x + difference.x,transform2D_->position2D_.y + difference.y};
			enemySprite_[enemyCount].first->transform_->rotation2D_ = (itr->objectPtr_->transform_->rotation_.y);
			enemySprite_[enemyCount].first->transform_->scale2D_ = { itr->objectPtr_->transform_->scale_.x / 20,itr->objectPtr_->transform_->scale_.z / 20 };

			//一定距離内にいるなら
			if (20 >= length)
			{
				enemySprite_[enemyCount].first->drawFlag_ = true;

			}
			enemySprite_[enemyCount].first->order_ = 1;
			enemyCount++;
		}

	}
}

bool MapUI::AABB(IFE::Float2 pos1, IFE::Float2 scale1, IFE::Float2 pos2, IFE::Float2 scale2)
{
	IFE::Float2 min1 = { pos1.x - scale1.x,pos1.y - scale1.y };
	IFE::Float2 max1 = { pos1.x + scale1.x,pos1.y + scale1.y };

	IFE::Float2 min2 = { pos2.x - scale2.x,pos2.y - scale2.y };
	IFE::Float2 max2 = { pos2.x + scale2.x,pos2.y + scale2.y };

	//中央からサイズ分の位置内にもう片方があるか
	if ((min1.x <= max2.x && max1.x >= min2.x) && (min1.y <= max2.y && max1.y >= min2.y))
	{
		//当たってますよ
		return true;
	}

	//当たってないよ
	return false;
}

bool MapUI::OBB(IFE::Vector2 pos1, IFE::Float2 scale1, IFE::Vector2 pos2, IFE::Float2 scale2, float angle1, float angle2)
{
	IFE::Vector2 axesX1 = { std::cos(angle1), std::sin(angle1) };
	IFE::Vector2 axesY1 = { -std::sin(angle1), std::cos(angle1) };

	IFE::Vector2 axesX2 = { std::cos(angle2), std::sin(angle2) };
	IFE::Vector2 axesY2 = { -std::sin(angle2), std::cos(angle2) };

	IFE::Vector2 axes[4] = { axesX1,axesY1 ,axesX2 ,axesY2 };
	
	for (const IFE::Vector2& axis : axes) 
	{
		float projection1 = GetProjectionRadius(scale1, axis, axesX1, axesY1);
		float projection2 = GetProjectionRadius(scale2, axis, axesX2, axesY2);
		float distance = std::fabs((pos1 - pos2).Dot(axis));

		if (distance > projection1 + projection2)
		{
			return false; //当たってねえよ
		}
	}

	return true; // たぶん当たった
}


float MapUI::GetProjectionRadius(IFE::Float2 scale, IFE::Vector2 Axis, IFE::Vector2 AxisX, IFE::Vector2 AxisY)
{
	return scale.x * std::fabs(Axis.Dot(AxisX)) + scale.y * std::fabs(Axis.Dot(AxisY));
}