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
	/// èâä˙âª
	/// </summary>
	void Initialize();

	/// <summary>
	/// çXêV
	/// </summary>
	void Update();

	/// <summary>
	/// èIóπ
	/// </summary>
	void Finalize();
};

