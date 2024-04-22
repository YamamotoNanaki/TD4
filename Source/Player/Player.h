#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"

class Player:public IFE::Component
{
private:

	bool modeFlag_ = false;//false‚Ì‚Æ‚«action

	PlayerAction* action_ = nullptr;
	PlayerDrone* drone_ = nullptr;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);

private:

	void ChangeMode();
};