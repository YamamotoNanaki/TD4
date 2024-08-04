#pragma once
#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"


class GimmickSwitchEvent :public IFE::IEvent
{
public:
	GimmickSwitchEvent();
	~GimmickSwitchEvent();

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

	std::string selectGimmickName_;

	IFE::Object3D* selectGimmickObject_ = nullptr;

	//切り替え
	bool isSwitch_ = false;

	int32_t selectNum = 0;

};