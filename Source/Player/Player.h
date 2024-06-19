#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include"UI.h"

class Player :public IFE::Component
{
private:

	bool modeFlag_ = false;//false‚Ì‚Æ‚«action

	PlayerAction* action_ = nullptr;
	PlayerDrone* drone_ = nullptr;

	IFE::UI* ui_ = nullptr;

	float droneHighlightingDistance_ = 20;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore collider);

	bool GetMode();
	void SetMode(bool flag);

private:

	void ChangeMode();

#ifdef EditorMode
	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;
};