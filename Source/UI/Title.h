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

	//タイトルロゴ
	float titleAnimationTimer_ = 0.0f;
	float beforeGameLogoPosY_ = 0.0f;

	//カメラ
	IFE::Camera* camera_ = nullptr;
	float cameraAngle_ = 0.0f;
	float beforeEaseAngle_ = 0.0f;
	float distanse_ = 100.0f;
	const float cDistance_ = 100.0f;

	//タイトルからセレクトへの遷移
	bool animationFlag_ = false;
	float animationTime_ = 0.0f;
	const float maxAnimationTime_ = 2.0f;

	//ステージ選択
	int8_t stageNum_ = 1;
	int8_t minStageNum_ = 1;
	int8_t maxStageNum_ = 3;
	IFE::Vector2 oldLAnalog_ = {};

	//フレーム
	bool flameAnimationFlag_ = false;
	float flameAnimationTime_ = 0.0f;
	const float maxFlameAnimationTime_ = 0.5f;
	IFE::Vector3 flamePos_[3];
	IFE::Vector3 oldFlamePos = {};

	//最終確認
	bool checkFlag_ = true;

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

	void SelectFlame();

	void StageNameUI();
};