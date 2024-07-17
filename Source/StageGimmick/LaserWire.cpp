#include "LaserWire.h"

#include "Transform.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "JsonManager.h"
#include "Player.h"
#include "EventFactory.h"
#include "IFETime.h"
#include "Sound.h"

void LaserWire::Initialize()
{
	cols_ = objectPtr_->GetComponent<IFE::Collider>();
	objects_.clear();
	for (int32_t i = 0; i < poss_.size(); i++)
	{
		IFE::Object3D* addObj=IFE::ObjectManager::Instance()->AddInitialize("wireobj", IFE::ModelManager::Instance()->GetModel("dice"));
		addObj->transform_->position_ = poss_[i];
		addObj->transform_->rotation_ = transform_->rotation_;
		addObj->transform_->scale_ = scales_[i];
		addObj->transform_->parent_ = objectPtr_->transform_;
		addObj->SetColor({ 1.0f,0,0,1.0f });
		objects_.push_back(addObj);
	}

	if(event_==nullptr)event_ = IFE::EventFactory::Instance()->CreateEventClass(EventName::EventString(eventType_));

	event_->Initialize();
	event_->InputTransform(*transform_);
	IFE::Sound::Instance()->LoadMP3("shockDam");
	IFE::Sound::Instance()->SetVolume("shockDam", 80);
	
}

void LaserWire::Update()
{
	for (int16_t i = 0; i < poss_.size(); i++)
	{
		auto col = cols_->GetCollider(i);
		if (col == nullptr)
		{
			col = cols_->AddCollider();
			col->SetColliderType(IFE::ColliderType::OBB);
			col->SetNoPushBackFlag(true);
			col->SetPushBackFlag(false);
		}
		col->SetOffsetPosition(poss_[i]);
		col->SetOffsetScale(scales_[i]);
	}

	if (countHitTimer_ > hitMaxTime_ && !isEventStart)
	{
		//イベントを起動
		event_->StartInitialize();
		isEventStart = true;
	}
	//起動したイベントのupdate
	if (isEventStart)
	{
		if (!event_->GetIsEnd())
		{
			event_->Update();
		}
		//elseにしてないのは終わったらすぐ終わってほしいから
		if (event_->GetIsEnd())
		{
			isEventStart = false;
			countHitTimer_ = 0;
		}
	}

	if (!isHit_)
	{
		countHitTimer_ = 0;
	}

	isHit_ = false;
}

void LaserWire::Draw()
{

}

void LaserWire::Finalize()
{

}

void LaserWire::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	if (hitCollider->objectPtr_->GetComponent<PlayerAction>())
	{
		isHit_ = true;
		countHitTimer_+=IFE::IFETime::sDeltaTime_;
		if (IFE::Sound::Instance()->GetPlayStatus("shockDam") == false) {
			IFE::Sound::Instance()->SoundPlay("shockDam", false, true);
		}
	}
	myCollider;
}

#ifdef EditorMode
#include "ImguiManager.h"
void LaserWire::ComponentDebugGUI()
{
	
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloat3GUI(&transform_->position_, "points", 0.25f, -1000, 1000);
	gui->DragVectorFloat3GUI(poss_, "pos", {0,0,0}, 0.25f, -1000, 1000);
	gui->DragVectorFloat3GUI(scales_, "scale", {0,0,0}, 0.25f, 1, 1000);
	

	if (oldposSize != poss_.size())
	{
		scales_.resize(poss_.size());
		oldposSize = (int32_t)poss_.size();
		oldscaleSize = (int32_t)scales_.size();
	}
	if (oldscaleSize != scales_.size())
	{
		poss_.resize(scales_.size());
		oldposSize = (int32_t)poss_.size();
		oldscaleSize = (int32_t)scales_.size();
	}

	std::vector<std::string>items;
	items.resize(EventType::EventCount);

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i] = EventTypeToString(EventType(i));
	}

	int32_t oldEventType = eventType_;
	//設定したいイベントの番号にする
	gui->Combo("event Type", eventType_, items);
	
	//当たってイベントが起きるまでの時間を設定
	gui->DragFloatGUI(&hitMaxTime_, "HitTime", 1.0f, 0, 1000);
	
	if (oldEventType != eventType_)
	{
		event_ = IFE::EventFactory().CreateEventClass(EventName::EventString(eventType_));
	}
	
	if (event_ != nullptr)
	{
		event_->DebugGUI();
	}
}

void LaserWire::OutputComponent(nlohmann::json& json)
{
	for (int32_t i = 0; i < poss_.size(); i++)
	{
		IFE::JsonManager::Instance()->OutputFloat3(json["pos"][i], poss_[i]);
		IFE::JsonManager::Instance()->OutputFloat3(json["scale"][i], scales_[i]);
	}

	IFE::Float2 output = { (float)eventType_,0 };

	IFE::JsonManager::Instance()->OutputFloat2(json["EventType"], output);

	IFE::Float2 output2 = { hitMaxTime_,0 };

	IFE::JsonManager::Instance()->OutputFloat2(json["hitMaxTime"], output2);

	event_->OutputData(json["EventSeting"]);

}
#endif

void LaserWire::LoadingComponent(nlohmann::json& json)
{
	poss_.clear();
	scales_.clear();
	int32_t count = 0;
	for (nlohmann::json& events : json["pos"])
	{
		IFE::Float3 addPos;
		addPos.x = events[0];
		addPos.y = events[1];
		addPos.z = events[2];
		poss_.push_back(addPos);
	}
	count = 0;
	for (nlohmann::json& events : json["scale"])
	{
		IFE::Float3 addScale;
		addScale.x = events[0];
		addScale.y = events[1];
		addScale.z = events[2];
		scales_.push_back(addScale);
	}

	eventType_ = json["EventType"][0];
	hitMaxTime_ = json["hitMaxTime"][0];

	event_ = IFE::EventFactory::Instance()->CreateEventClass(EventName::EventString(eventType_));

	event_->InputData(json["EventSeting"]);
}

std::string LaserWire::EventTypeToString(EventType eventType)
{
	switch (eventType) {
	case EventType::damage:   return "damage";
	case EventType::door: return "door";
	case EventType::DroneKeepoutZone: return "droneKeepoutZone";
	case EventType::EnemySpawn: return "EnemySpawn";
	default:    return "UNKNOWN";
	}
}
