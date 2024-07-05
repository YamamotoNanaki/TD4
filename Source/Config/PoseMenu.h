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

class PoseMenu :public IFE::Component
{
private:

	bool poseFlag_ = false;
	PoseSelect selectFlag_ = PoseSelect::RETURNGAME;

public:

	PoseMenu();
	~PoseMenu();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	bool GetPoseFlag();
};

