#pragma once
#include "Component.h"
#include"IFEMath.h"
#include"CameraManager.h"
#include"PlayerAttack.h"

class PlayerAction : public IFE::Component
{
private:

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };

	//正面ベクトルの作成
	IFE::Vector3 cameraFrontVec_ = {};
	IFE::Vector3 frontVec_ = {};
	//仮ベクトル
	IFE::Vector3 temporaryVec_ = { 0,1,0 };
	//右ベクトルの作成
	IFE::Vector3 rightVec_ = {};

	uint8_t hp = 10;

	//攻撃判定クラス
	PlayerAttack* playerAttack_ = nullptr;

#pragma region カメラ
	IFE::Vector3 cameraPos_ = {};
	IFE::Vector2 cameraAngle_ = {};

	IFE::Camera* actionCamera_ = nullptr;
#pragma endregion カメラ

	bool attackFlag_ = false;
	const uint8_t attackTime_ = 10;
	uint8_t attackTimer_ = 0;

	const uint16_t controllerRange_ = 10000;

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

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)override;


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
	/// カメラの更新処理
	/// </summary>
	void CameraUpdate();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// カメラ補間
	/// </summary>
	void CameraComplement();

	/// <summary>
	/// カメラの回転処理
	/// </summary>
	void CameraRot();
};