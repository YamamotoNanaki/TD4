#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"
enum class PoseSelectFlag
{
	RETURNGAME,
	RETURNTITLE,
	CONFIG
};

class PoseMenu :public IFE::Component
{
private:

	bool poseFlag_ = false;
	PoseSelectFlag selectFlag_ = PoseSelectFlag::RETURNGAME;
	uint8_t selectNum_ = 0;
	uint8_t minSelectNum_ = 0;
	uint8_t maxSelectNum_ = 2;

	float oldLAnalog_ = 0.0f;

public:

	PoseMenu();
	~PoseMenu();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	bool GetPoseFlag();

private:

	void PoseChange();

	void PoseSelect();
};

