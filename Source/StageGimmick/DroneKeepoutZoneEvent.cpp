#include "DroneKeepoutZoneEvent.h"
#include "ObjectManager.h"
#include "Collider.h"
#include "ModelManager.h"
#include "Transform.h"

DroneKeepoutZoneEvent::DroneKeepoutZoneEvent()
{
}

DroneKeepoutZoneEvent::~DroneKeepoutZoneEvent()
{
}


void DroneKeepoutZoneEvent::Initialize()
{
	if (zoneObject_ == nullptr)
	{
		zoneObject_ = IFE::ObjectManager::Instance()->AddInitialize("zoneObject", IFE::ModelManager::Instance()->GetModel("dice"));
		zoneObject_->transform_->position_ = pos_;

		//適当に黄色っぽい色にしてるけど気になるなら変える
		zoneObject_->SetColor({ 0.5f,0.7f,0.0f,0.2f });
	}

	if (!zoneObject_->GetComponent<IFE::Collider>())
	{
		zoneObject_->AddComponentBack<IFE::Collider>();
		auto col = zoneObject_->GetComponent<IFE::Collider>();
		auto c = col->AddCollider();
		c->SetColliderType(IFE::ColliderType::OBB);
		c->SetPushBackFlag(true);
	}
}

void DroneKeepoutZoneEvent::Update()
{
	if (!isEnd_)
	{
		
	}
}
#ifdef EditorMode
#include "ImguiManager.h"
void DroneKeepoutZoneEvent::DebugGUI()
{
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloat3GUI(&pos_, "DroneKeepoutZonePos", 0.25f, -1000, 1000);
	gui->DragFloat3GUI(&scele_, "DroneKeepoutZoneScele", 1.0f, 1, 100);

}
#endif

void DroneKeepoutZoneEvent::OutputData(nlohmann::json& json)
{
	IFE::JsonManager::Instance()->OutputFloat3(json["DroneKeepoutZonePos"], pos_);
	IFE::JsonManager::Instance()->OutputFloat3(json["DroneKeepoutZoneScele"], scele_);
}

void DroneKeepoutZoneEvent::InputData(nlohmann::json& json)
{

	pos_.x = json["DroneKeepoutZonePos"][0];
	pos_.y = json["DroneKeepoutZonePos"][1];
	pos_.z = json["DroneKeepoutZonePos"][2];

	scele_.x = json["DroneKeepoutZoneScele"][0];
	scele_.y = json["DroneKeepoutZoneScele"][1];
	scele_.z = json["DroneKeepoutZoneScele"][2];

}

void DroneKeepoutZoneEvent::StartInitialize()
{
	isEnd_ = false;
}