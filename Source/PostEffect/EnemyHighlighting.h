#pragma once
#include "IPostEffect.h"
#include "IFEMath.h"
#include "Camera.h"

class EnemyHighlighting : public IFE::IPostEffect
{
private:
	IFE::Float3* dronePosition;
	IFE::Camera* droneCamera_ = nullptr;
	IFE::GraphicsPipeline* enemyJuge = nullptr;
	IFE::GraphicsPipeline* enemyAnimJuge = nullptr;
	IFE::Object3D* playerAction = nullptr;
	IFE::Object3D* playerDrone = nullptr;

	struct Judge
	{
		bool enemy = true;
		IFE::Float3 pad;
	};
	std::unique_ptr<IFE::ConstBuffer<Judge>>cb;
	std::unique_ptr<IFE::ConstBuffer<Judge>>cbFalse;

public:
	float droneHighlightingDistance_ = 20;

public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
};

