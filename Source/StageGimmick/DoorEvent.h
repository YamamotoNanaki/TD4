#pragma once

#include "IEvent.h"
#include "IFEMath.h"


class DoorEvent :public IFE::IEvent
{
public:
	DoorEvent();
	~DoorEvent();

	//初期化
	void Initialize()override;

	//毎フレーム更新
	void Update()override;

	//デバック更新
	void DebugGUI()override;

	//設定を出す
	void DebugGUI()override;

	//使うギミック側でデータを出したいとき
	void OutputData(nlohmann::json& json)override;

	//使うギミック側でデータを受け取りたいとき
	void InputData(nlohmann::json& json)override;

private:

	//開始位置
	IFE::Float3 doorStartPos_;

	//終了位置
	IFE::Float3 doorEndPos_;

	//開いているか
	bool isOpen_ = false;

	//開く時間
	float moveOpenTime_ = 1;

	//閉じる時間
	float moveCloseTime_ = 1;


};