#pragma once
#include "IPostEffect.h"
#include "IFEMath.h"
#include "Camera.h"

class EnemyHighlighting : public IFE::IPostEffect
{
private:
	IFE::Float3* dronePosition;
	IFE::Camera* droneCamera_ = nullptr;

public:
	float droneHighlightingDistance_ = 20;

public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
};

