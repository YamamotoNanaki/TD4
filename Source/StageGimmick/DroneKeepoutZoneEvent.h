#pragma once
#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"

class DroneKeepoutZoneEvent :public IFE::IEvent
{
public:
	DroneKeepoutZoneEvent();
	~DroneKeepoutZoneEvent();

	//初期化
	void Initialize()override;

	//毎フレーム更新
	void Update()override;

	//デバック更新
	void DebugGUI()override;

	//使うギミック側でデータを出したいとき
	void OutputData(nlohmann::json& json)override;

	//使うギミック側でデータを受け取りたいとき
	void InputData(nlohmann::json& json)override;

	//開始時の初期化
	void StartInitialize()override;

private:

	//開始位置
	IFE::Float3 pos_;

	//終了位置
	IFE::Float3 scele_ = {1,1,1};

	//禁止ゾーンの本体
	IFE::Object3D* zoneObject_ = nullptr;


};