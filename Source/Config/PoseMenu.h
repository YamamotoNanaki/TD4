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
	int8_t selectNum_ = 0;
	int8_t oldSelectNum_ = 0;
	int8_t minSelectNum_ = 0;
	int8_t maxSelectNum_ = 2;

	float oldLAnalog_ = 0.0f;

	bool configFlag_ = false;

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

	bool GetCofigFlag();
	void SetConfigFlag(bool flag);

	void Reset();

private:

	void PoseChange();

	void PoseSelect();

	void Select();
};

