#pragma once
#include "JsonManager.h"
#include "Transform.h"

//イベントの種類用意はしたけど置き場所に困ってる
enum EventType
{
	damage,
	door,
	DroneKeepoutZone,
	EnemySpawn,
	GimmickSwitch,
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
		else if (eventNum == EventType::DroneKeepoutZone)
		{
			return "DroneKeepoutZoneEvent";
		}
		else if (eventNum == EventType::EnemySpawn)
		{
			return "EnemySpawnEvent";
		}
		else if (eventNum == EventType::GimmickSwitch)
		{
			return "GimmickSwitchEvent";
		}

		return "";
	}
};

//enumからstringを取得するための関数
std::string EventTypeToString(EventType eventType);

namespace IFE {
	class IEvent
	{
	public:
		virtual ~IEvent() {};

		//初期化
		virtual void Initialize() {};

		//終了処理
		virtual void Finalize() {};

		//毎フレーム更新
		virtual void Update() {};

		//終了フラグ獲得
		virtual bool GetIsEnd() { return isEnd_; };

		//設定を出す
		virtual void DebugGUI() {};

		//使うギミック側でデータを出したいとき
		virtual void OutputData(nlohmann::json& json) { json; };

		//使うギミック側でデータを受け取りたいとき
		virtual void InputData(nlohmann::json& json) { json; };

		//ギミック側の位置が欲しいとき
		virtual void InputTransform(const IFE::Transform& transform) { transform; };

		//開始時の初期化
		virtual void StartInitialize() {};
		

	protected:

		//このイベントが終わっているか
		bool isEnd_ = false;

	};


}
