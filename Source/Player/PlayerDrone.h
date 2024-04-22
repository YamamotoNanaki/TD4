#pragma once
#include <COmponent.h>
#include"IFEMath.h"

class PlayerDrone :public IFE::Component
{
private:

	IFE::Vector3 speed_ = {};

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

	bool GetDrawFlag();
	void SetDrawFlag(bool flag);

	void SetPos(const IFE::Vector3& pos);
};