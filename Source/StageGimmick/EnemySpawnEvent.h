#pragma once
#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"

class EnemySpawnEvent :public IFE::IEvent
{
public:
	EnemySpawnEvent();
	~EnemySpawnEvent();

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

	//ギミック側の位置が欲しいとき
	void InputTransform(const IFE::Transform& transform)override;

private:

	//湧かせる位置
	IFE::Float3 spawnPos_;

	//トラップの位置
	IFE::Float3 trapPos_;

	//湧かす敵の数
	int32_t enemySpawnNum_ = 1;

	//湧かせた敵の数
	int32_t enemyNum_ = 0;

	//敵の上限
	int32_t MaxEnemyNum_ = 10;

	float enemySpawnCT_ = 0;
	float enemySpawnMaxCT_ = 10;

	IFE::Object3D* EnemyManagerObject_ = nullptr;


};