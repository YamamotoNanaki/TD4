#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include"UI.h"
#include"PostEffectManager.h"
#include"EnemyHighlighting.h"
#include"DronePostEffect.h"
#include"DroneRecoveryUI.h"
#include"PoseMenu.h"
#include "Tutorial.h"

class Player :public IFE::Component
{
private:

	bool modeFlag_ = false;//falseのときaction

	PlayerAction* action_ = nullptr;
	PlayerDrone* drone_ = nullptr;

	IFE::UI* ui_ = nullptr;
	DroneRecoveryUI* droneRecoveryUI_ = nullptr;

	float droneHighlightingDistance_ = 20;

	IFE::IPostEffect* enemyHilight_;
	IFE::IPostEffect* dronePostEffect_;

#pragma region ドローンの回収系

	bool droneRecoveryFlag_ = false;
	bool firstRecovery = false;
	float droneRecoverytime_ = 0.0f;
	const float maxDroneRecoverytime_ = 3.0f;

#pragma endregion ドローンの回収系

#pragma region カメラの変更関連

public:
	struct CameraChangeParam
	{
		//カメラ切替演出かどうかのフラグ
		bool cameraChange = false;
		//カメラの切り替えが完了したかのフラグ
		bool change = false;
		float cameraChangeTimer = 0;
		//演出の終了時間
		float cameraChangeMaxTime = 1.0f;
		//カメラの実際の切り替えタイミング
		float changeTime = 0.6f;
	};
private:
	CameraChangeParam ccp_;
	bool oldIsDamageFlag_ = false;
#pragma endregion

	PoseMenu* pose_ = nullptr;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore collider);

	void DroneBreak();

	bool GetMode();
	bool GetFirstRecovery();
	void SetMode(bool flag);
	inline CameraChangeParam* GetCameraChangeParam() { return &ccp_; }

private:

	void ChangeMode();

	/// <summary>
	/// ドローンの回収処理
	/// </summary>
	void DroneRecovery();

#ifdef EditorMode
	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;
};