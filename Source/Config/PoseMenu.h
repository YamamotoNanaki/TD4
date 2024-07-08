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

	bool configFlag_ = false;

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

	bool GetCofigFlag();
	void SetConfigFlag(bool flag);

private:

	void PoseChange();

	void PoseSelect();

	void Select();

	void Reset();
};

