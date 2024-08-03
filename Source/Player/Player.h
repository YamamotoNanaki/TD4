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

	bool modeFlag_ = false;//false�̂Ƃ�action

	PlayerAction* action_ = nullptr;
	PlayerDrone* drone_ = nullptr;

	IFE::UI* ui_ = nullptr;
	DroneRecoveryUI* droneRecoveryUI_ = nullptr;

	float droneHighlightingDistance_ = 20;

	IFE::IPostEffect* enemyHilight_;
	IFE::IPostEffect* dronePostEffect_;

#pragma region �h���[���̉���n

	bool droneRecoveryFlag_ = false;
	bool firstRecovery = false;
	float droneRecoverytime_ = 0.0f;
	const float maxDroneRecoverytime_ = 3.0f;

#pragma endregion �h���[���̉���n

#pragma region �J�����̕ύX�֘A

public:
	struct CameraChangeParam
	{
		//�J�����ؑ։��o���ǂ����̃t���O
		bool cameraChange = false;
		//�J�����̐؂�ւ��������������̃t���O
		bool change = false;
		float cameraChangeTimer = 0;
		//���o�̏I������
		float cameraChangeMaxTime = 1.0f;
		//�J�����̎��ۂ̐؂�ւ��^�C�~���O
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
	/// �h���[���̉������
	/// </summary>
	void DroneRecovery();

#ifdef EditorMode
	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;
};