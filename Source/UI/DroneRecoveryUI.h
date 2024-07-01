#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"

class DroneRecoveryUI :public IFE::Component
{
private:

public:

	void Initialize();

	void Update();

	void Recovery(float time, float maxTime);

	void SetDrawFlag(bool flag);
};

