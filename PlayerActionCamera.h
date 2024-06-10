#pragma once
#include "Component.h"
#include"CameraManager.h"

class PlayerActionCamera:public IFE::Component
{
private:

	IFE::Camera* actionCamera_ = nullptr;

	IFE::Vector3 cameraPos_ = {};
	IFE::Vector2 cameraAngle_ = {};

	//ターゲットからの距離
	float distance_ = 15.0f;

	const uint16_t controllerRange_ = 10000;
	bool ColliderHitFlag_ = false;

public:

	void Initialize();

	void Update();

	void CameraUpdate(const IFE::Vector3& playerPos);

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)override;

	IFE::Float3 GetPos();

	IFE::Camera* GetCamera();

private:

	void CameraMove(const IFE::Vector3& playerPos);

	void CameraRot();
};