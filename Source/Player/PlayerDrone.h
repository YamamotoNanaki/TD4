#pragma once
#include <COmponent.h>
#include"IFEMath.h"

class PlayerDrone :public IFE::Component
{
private:

	IFE::Vector3 speed_ = {};

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };
	IFE::Vector3 frontVec_ = {};

	IFE::Camera* droneCamera_ = nullptr;
	bool cameraMode_ = true;

	void SpeedZero(float& speed);

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);

	void MoveUpdate();
	void Move();
	void Rotation();

	void CameraUpdate();

	bool GetDrawFlag();
	void SetDrawFlag(bool flag);

	void SetPos(const IFE::Vector3& pos);
};