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
	float droneRecoverytime_ = 0.0f;
	const float maxDroneRecoverytime_ = 3.0f;

#pragma endregion �h���[���̉���n

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore collider);

	void DroneBreak();

	bool GetMode();
	void SetMode(bool flag);

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