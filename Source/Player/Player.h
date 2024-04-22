#pragma once
#include"Component.h"
#include"IFEMath.h"

class Player:public IFE::Component
{
private:

	bool modeFlag_ = false;//false‚Ì‚Æ‚«action


public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);

private:

	void ChangeMode();
};