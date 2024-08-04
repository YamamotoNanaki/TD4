#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"
#include"Player.h"

class ClearAnimation :public IFE::Component
{
private:

	Player* player_ = nullptr;
	PlayerAction* action_ = nullptr;
	PlayerActionCamera* actionCamera_ = nullptr;

	//アニメーション関連
	bool animationFlag_ = false;
	float animationTimer_ = 0.0f;
	const float maxAnimationTimer_ = 5.0f;
	bool startFlag_ = false;

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

	void SetAnimationFlag(bool flag);
	bool GetAnimationFlag();

private:

	void AnimationInit();
	void PlayerMove();
	void CameraMove();
	void CameraRot();

	void SceneChange();
};