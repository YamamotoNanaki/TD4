#pragma once
#include <COmponent.h>
#include"IFEMath.h"

class PlayerDrone :public IFE::Component
{
private:

	IFE::Vector3 speed_ = {};

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };
	IFE::Vector3 frontVec_ = {};

	IFE::Camera* droneCamera_ = nullptr;
	bool cameraMode_ = false;

	const uint16_t controllerRange_ = 10000;

	/// <summary>
	/// 段々speedを0にする処理
	/// </summary>
	/// <param name="speed"></param>
	void SpeedZero(float& speed);

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

	void OnColliderHit(IFE::ColliderCore collider);

	/// <summary>
	/// Playerクラスで呼ぶ操作全般更新処理
	/// </summary>
	void MoveUpdate();
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

	bool GetDrawFlag();
	void SetDrawFlag(bool flag);

	void SetPos(const IFE::Vector3& pos);
};