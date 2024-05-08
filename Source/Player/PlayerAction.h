#pragma once
#include "Component.h"
#include"IFEMath.h"

class PlayerAction : public IFE::Component
{
private:

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };
	IFE::Vector3 frontVec_ = {};

	bool attackFlag_ = false;
	const uint8_t attackTime_ = 10;
	uint8_t attackTimer_ = 0;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);

	void MoveUpdate();

	const IFE::Vector3 GetPos();

private:

	void Move();

	void Rotation();

	void Attack();
};