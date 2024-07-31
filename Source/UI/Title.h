#pragma once
#include "Component.h"

enum class TitleSelect
{
	TITLE,
	SELECT,
	SELECT2
};

class Title : public IFE::Component
{
private:

	TitleSelect titleSelectFlag_ = TitleSelect::TITLE;

	//�^�C�g�����S
	float titleAnimationTimer_ = 0.0f;
	float beforeGameLogoPosY_ = 0.0f;

	//�J����
	IFE::Camera* camera_ = nullptr;
	float cameraAngle_ = 0.0f;
	float beforeEaseAngle_ = 0.0f;
	float distanse_ = 120.0f;

	//�^�C�g������Z���N�g�ւ̑J��
	bool animationFlag_ = false;
	float animationTime_ = 0.0f;
	const float maxAnimationTime_ = 2.0f;

	//�X�e�[�W�I��
	int8_t stageNum_ = 0;
	int8_t minStageNum_ = 1;
	int8_t maxStageNum_ = 3;
	float oldLAnalog_ = 0.0f;

	//�ŏI�m�F
	bool checkFlag_ = false;

public:
	void Initialize();
	void Update();
	void SetIsNext(bool isNext_) { isNext = isNext_; };
private:
	bool isNext;

private:

	void TitleLogoSimpleHarmonicMotion();
	void CameraRot();

	void ToSelectAnimation();
	void BackTitleAnimation();

	void Select();
	void SelectCheck();

	void ImGUI();
};