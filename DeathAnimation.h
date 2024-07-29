#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"

class DeathAnimation :public IFE::Component
{
private:

	//死亡関連
	bool deathAnimationFlag_ = false;
	bool deathAnimationEndFlag_ = false;
	float dethaAnimationTime_ = 0.0f;
	bool deathFadeAnimationFlag_ = false;
	float deathFadeAnimationTime_ = 0.0f;
	const float maxDeathFadeAnimationTime_ = 2.0f;

	//選択
	bool isContinue_ = true;

	const uint16_t controllerRange_ = 10000;
	float oldLAnalog_ = 0.0f;

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	void SetDeathAnimationFlag(bool flag);

private:

	void Fade();

	void Select();
};