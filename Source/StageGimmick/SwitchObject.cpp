#include "SwitchObject.h"
#include "Transform.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "JsonManager.h"
#include "Player.h"
#include "EventFactory.h"
#include "IFETime.h"
#include "Sound.h"
#include "Input.h"

void SwitchObject::Initialize()
{

	if (!objectPtr_->GetComponent<IFE::Collider>())
	{
		objectPtr_->AddComponentBack<IFE::Collider>();
		auto col = objectPtr_->GetComponent<IFE::Collider>();
		auto c = col->AddCollider();
		c->SetColliderType(IFE::ColliderType::OBB);
		c->SetNoPushBackFlag(true);
		c->SetPushBackFlag(false);
	}
	cols_ = objectPtr_->GetComponent<IFE::Collider>();
	if (cols_ != nullptr)
	{
		auto col = objectPtr_->GetComponent<IFE::Collider>();
		auto c = col->AddCollider();
		c->SetColliderType(IFE::ColliderType::OBB);
		c->SetNoPushBackFlag(true);
		c->SetPushBackFlag(false);
		cols_ = objectPtr_->GetComponent<IFE::Collider>();
	}

	if (event_ == nullptr)event_ = IFE::EventFactory::Instance()->CreateEventClass(EventName::EventString(eventType_));

	event_->Initialize();
	event_->InputTransform(*transform_);

}

void SwitchObject::Update()
{
	cols_->GetCollider(0)->SetNoPushBackFlag(true);
	if (isActive_)
	{
		cols_->GetCollider(0)->SetOffsetPosition(objectPtr_->transform_->position_);
		//設定したサイズをセット
		cols_->GetCollider(0)->SetOffsetScale(scale_);

		if (countHitTimer_ > hitMaxTime_ && (IFE::Input::GetKeyPush(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::X)))
		{
			isSwitchObject_ = true;
		}

		if (isSwitchObject_ && !isEventStart)
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
				isSwitchObject_ = false;
				countHitTimer_ = 0;
			}
		}

		if (!isHit_)
		{
			countHitTimer_ = 0;
		}

		isHit_ = false;
	}
	else
	{
		isHit_ = false;
	}
}

void SwitchObject::Draw()
{

}

void SwitchObject::Finalize()
{

}

void SwitchObject::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	if (hitCollider->objectPtr_->GetComponent<PlayerAction>() && isActive_)
	{
		isHit_ = true;
		countHitTimer_ += IFE::IFETime::sDeltaTime_;
	}
	myCollider;
}

#ifdef EditorMode
#include "ImguiManager.h"
void SwitchObject::ComponentDebugGUI()
{

	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();

	//当たり判定の大きさ設定
	gui->DragFloat3GUI(&scale_, "scale",0.25f, 1, 1000);

	gui->CheckBoxGUI(&testflag_, "isActive");
	SetIsActive(testflag_);
	

	std::vector<std::string>items;
	items.resize(EventType::EventCount);

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i] = EventTypeToString(EventType(i));
	}

	int32_t oldEventType = eventType_;
	//設定したいイベントの番号にする
	gui->Combo("event Type", eventType_, items);

	//当たってイベントが起動できるようになるまでの時間を設定
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

void SwitchObject::OutputComponent(nlohmann::json& json)
{
	

	IFE::Float2 output = { (float)eventType_,(float)isActive_ };

	IFE::JsonManager::Instance()->OutputFloat2(json["EventType"], output);

	IFE::Float2 output2 = { hitMaxTime_,0 };

	IFE::JsonManager::Instance()->OutputFloat2(json["hitMaxTime"], output2);


	IFE::JsonManager::Instance()->OutputFloat3(json["collisionScale"], scale_);

	event_->OutputData(json["EventSeting"]);

}
#endif

void SwitchObject::LoadingComponent(nlohmann::json& json)
{
	
	eventType_ = json["EventType"][0];
	float a = json["EventType"][1];
	isActive_ = a;
	hitMaxTime_ = json["hitMaxTime"][0];

	scale_.x = json["collisionScale"][0];
	scale_.y = json["collisionScale"][1];
	scale_.z = json["collisionScale"][2];

	event_ = IFE::EventFactory::Instance()->CreateEventClass(EventName::EventString(eventType_));

	event_->InputData(json["EventSeting"]);
}

void SwitchObject::SetIsActive(bool flag)
{
	isActive_ = flag;
	if (cols_ != nullptr)
	{
		cols_->GetCollider(0)->active_ = flag;
	}
}
