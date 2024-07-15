#pragma once
#include "Component.h"
#include"CameraManager.h"

class PlayerActionCamera:public IFE::Component
{
private:

	IFE::Camera* actionCamera_ = nullptr;
	IFE::Object3D* player_ = nullptr;

	IFE::Vector3 cameraPos_ = {};
	IFE::Vector2 cameraAngle_ = {};

	//��Ԏ��Ԓ����l
	const float adjustedTimeValue_ = 15.0f;
	//�J������Y���W���ߒl
	const float cameraYAdd_ = 3.0f;

	//�^�[�Q�b�g����̋���
	float distance_ = 4.0f;
	float defaultDistance_ = 5.f;
	float defaultRayDistance_ = 0;

	const uint16_t controllerRange_ = 10000;
	bool ColliderHitFlag_ = false;

public:

	void Initialize();

	void CameraInitialize(const IFE::Vector3& playerPos);

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