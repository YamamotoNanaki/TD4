#pragma once
#include "Component.h"
#include"IFEMath.h"

class PlayerAction : public IFE::Component
{
private:

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };
	IFE::Vector3 frontVec_ = {};

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);

	void Move();

	const IFE::Vector3 GetPos();

private:

	void Rotation();
};