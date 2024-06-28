#pragma once
#include "Component.h"
#include "Object3D.h"
#include "Collider.h"
#include "IEvent.h"

//イベントの種類用意はしたけど置き場所に困ってる
enum EventType
{
	damage,
	door,
	EventCount//最大値、最後においてください
};

struct EventName
{
	static std::string EventString(int32_t eventNum) 
	{
		if (eventNum == EventType::damage)
		{
			return "DamageEvent";
		}
		else if (eventNum == EventType::door)
		{
			return "DoorEvent";
		}

		return "";
	}
};

class LaserWire:public IFE::Component
{
public:

	//現在当たっているか
	bool isHit_ = false;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;


private:

	IFE::Float3 pos_;

	std::vector<IFE::Float3> poss_;
	std::vector<IFE::Float3> rots_;
	std::vector<IFE::Float3> scales_;

	std::vector<IFE::Object3D*> objects_;

	//テストで置く描画用のオブジェクトなんかいい感じに追加する方法あるなら直す
	IFE::Object3D testobj_[10];

	int32_t oldposSize = 0;
	int32_t oldscaleSize = 0;

	IFE::Collider* cols_;

	float countHitTimer_ = 0;
	float hitMaxTime_ = 0;

	std::unique_ptr<IFE::IEvent> event_;

	//いったんイベントの種類を記録するために用意
	int32_t eventType_ = EventType::damage;

	bool isEventStart = false;

private:
	//enumからstringを取得するための関数
	std::string EventTypeToString(EventType eventType);
};

