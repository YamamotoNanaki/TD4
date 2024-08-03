#pragma once

#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"


class DoorEvent :public IFE::IEvent
{
public:
	DoorEvent();
	~DoorEvent();

	//初期化
	void Initialize()override;

	//毎フレーム更新
	void Update()override;

#ifdef EditorMode
	//デバック更新
	void DebugGUI()override;

	//使うギミック側でデータを出したいとき
	void OutputData(nlohmann::json& json)override;
#endif

	//使うギミック側でデータを受け取りたいとき
	void InputData(nlohmann::json& json)override;

	//開始時の初期化
	void StartInitialize()override;

private:

	//開始位置
	IFE::Float3 doorStartPos_;

	//終了位置
	IFE::Float3 doorEndPos_;

	//ドアのサイズ
	IFE::Float3 doorScele_;

	IFE::Object3D* doorObject_=nullptr;

	//開いているか
	bool isOpen_ = false;

	//カウント
	float moveTimer_ = 0;

	//開く時間
	float moveOpenTime_ = 1;

	//閉じる時間
	float moveCloseTime_ = 1;


};