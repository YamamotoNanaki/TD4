#pragma once
#include "IPostEffect.h"
#include "IFEMath.h"

class EnemyHighlighting : public IFE::IPostEffect
{
private:
	IFE::Float3* dronePosition;

public:
	float droneHighlightingDistance_ = 20;

public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
};

