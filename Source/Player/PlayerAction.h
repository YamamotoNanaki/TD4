#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"CameraManager.h"
#include"PlayerAttack.h"
#include"PlayerHp.h"
#include"SpriteManager.h"
#include"PlayerActionCamera.h"

class PlayerAction : public IFE::Component
{
private:

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };

	//正面ベクトルの作成
	IFE::Vector3 frontVec_ = {};
	//仮ベクトル
	IFE::Vector3 temporaryVec_ = { 0,1,0 };
	//右ベクトルの作成
	IFE::Vector3 rightVec_ = {};

	float rotY_ = 0.0f;

	uint8_t hp_ = 10;
	bool isHit_ = false;
	float hitTime_;
	const int8_t HIT_COOLTIME = 1;

	//PlayerHp
	IFE::PlayerHp* playerHp_ = nullptr;

	//攻撃判定クラス
	PlayerAttack* playerAttack_ = nullptr;

#pragma region カメラ

	PlayerActionCamera* camera_ = nullptr;
	
#pragma endregion カメラ

	bool attackFlag_ = false;
	const uint8_t attackTime_ = 10;
	uint8_t attackTimer_ = 0;

	const uint16_t controllerRange_ = 10000;

	//実際の方向ベクトル
	IFE::Vector3 actualFrontVec_ = { 0,0,1.0f };
	//入力されている方向
	IFE::Vector3 targetVec_ = { 0,0,0 };

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

	/// <summary>
	/// 体力減少
	/// </summary>
	void DecHp();

	/// <summary>
	/// Playerクラスで呼ぶ操作全般更新処理
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// positionの取得
	/// </summary>
	/// <returns></returns>
	const IFE::Vector3 GetPos();

	/// <summary>
	/// アクションカメラ取得
	/// </summary>
	/// <returns></returns>
	IFE::Camera* GetActionCamera();

	/// <summary>
	/// 攻撃フラグの取得
	/// </summary>
	/// <returns></returns>
	const bool GetAttackFlag();

	const IFE::Vector3 GetFrontVec();

	const float GetRotY();

	//Getter
	bool GetIsHit() { return isHit_; }

private:

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	// 目標値に近づける関数
	void approachTarget(float& current, float target, float step);
};