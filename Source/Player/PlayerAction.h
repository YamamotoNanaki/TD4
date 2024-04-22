#pragma once
#include "Component.h"
#include"IFEMath.h"

class PlayerAction : public IFE::Component
{
private:

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);

	void Move();

	const IFE::Vector3 GetPos();
};