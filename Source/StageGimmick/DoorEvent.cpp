#include "DoorEvent.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "CollideManager.h"
#include "Ease.h"
#include "Collider.h"
#include "Transform.h"

DoorEvent::DoorEvent()
{
	
}

DoorEvent::~DoorEvent()
{
}


void DoorEvent::Initialize()
{
	
	if (doorObject_ == nullptr)
	{
		//ダイスにしてるけどドアのモデルなどができたら置き換えたいっすね
		doorObject_ = IFE::ObjectManager::Instance()->AddInitialize("doorObj", IFE::ModelManager::Instance()->GetModel("dice"));
		doorObject_->transform_->position_ = doorStartPos_;
	}

	if (!doorObject_->GetComponent<IFE::Collider>())
	{
		doorObject_->AddComponentBack<IFE::Collider>();
		auto col = doorObject_->GetComponent<IFE::Collider>();
		auto c = col->AddCollider();
		c->SetColliderType(IFE::ColliderType::OBB);
		c->SetNoPushBackFlag(false);
		c->SetPushBackFlag(false);
	}

	
	
}

void DoorEvent::Update()
{
	if (!isEnd_)
	{
		//開いていないなら
		if (!isOpen_)
		{
			doorObject_->transform_->position_ = IFE::OutQuadFloat3(doorStartPos_, doorEndPos_, moveOpenTime_, moveTimer_);

			if (moveOpenTime_ > moveTimer_)
			{
				moveTimer_ += IFE::IFETime::sDeltaTime_;
			}
			else
			{
				moveTimer_ = 0;
				isEnd_ = true;
				isOpen_ = true;
			}
		}
		else
		{
			doorObject_->transform_->position_ = IFE::OutQuadFloat3(doorEndPos_, doorStartPos_, moveCloseTime_, moveTimer_);

			if (moveCloseTime_ > moveTimer_)
			{
				moveTimer_ += IFE::IFETime::sDeltaTime_;
			}
			else
			{
				moveTimer_ = 0;
				isEnd_ = true;
				isOpen_ = false;
			}
		}
	}
}
#ifdef EditorMode
#include "ImguiManager.h"
void DoorEvent::DebugGUI()
{
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloat3GUI(&doorStartPos_, "doorStartPos", 0.25f, -1000, 1000);
	gui->DragFloat3GUI(&doorEndPos_, "doorEndPos", 0.25f, -1000, 1000);

	gui->DragFloat3GUI(&doorScele_, "doorScele", 0.25f, -1000, 1000);

	gui->DragFloatGUI(&moveOpenTime_, "moveOpenTime", 1.0f, 0, 1000);
	gui->DragFloatGUI(&moveCloseTime_, "moveCloseTime", 1.0f, 0, 1000);

}
#endif

void DoorEvent::OutputData(nlohmann::json& json)
{
	IFE::JsonManager::Instance()->OutputFloat3(json["doorStartPos"], doorStartPos_);
	IFE::JsonManager::Instance()->OutputFloat3(json["doorEndPos"], doorEndPos_);
	IFE::JsonManager::Instance()->OutputFloat3(json["doorScele"], doorScele_);

	IFE::Float2 output = { moveOpenTime_, moveCloseTime_ };

	IFE::JsonManager::Instance()->OutputFloat2(json["moveDoorTime"], output);

}

void DoorEvent::InputData(nlohmann::json& json)
{
	
	doorStartPos_.x = json["doorStartPos"][0];
	doorStartPos_.y = json["doorStartPos"][1];
	doorStartPos_.z = json["doorStartPos"][2];

	doorEndPos_.x = json["doorEndPos"][0];
	doorEndPos_.y = json["doorEndPos"][1];
	doorEndPos_.z = json["doorEndPos"][2];

	doorScele_.x = json["doorScele"][0];
	doorScele_.y = json["doorScele"][1];
	doorScele_.z = json["doorScele"][2];

	moveOpenTime_ = json["moveDoorTime"][0];
	moveCloseTime_ = json["moveDoorTime"][1];

}

void DoorEvent::StartInitialize()
{
	isEnd_ = false;
}