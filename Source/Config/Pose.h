#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"
enum class PoseSelect
{
	RETURNGAME,
	RETURNTITLE,
	CONFIG
};

class Pose :public IFE::Component
{
private:

	bool poseFlag_ = false;
	PoseSelect selectFlag_ = PoseSelect::RETURNGAME;

public:

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
};

