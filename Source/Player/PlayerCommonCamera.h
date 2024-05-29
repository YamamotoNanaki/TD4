#pragma once
#include "Component.h"
#include "Collider.h"

class PlayerCommonCamera : public IFE::Component
{
private:
	IFE::Collider* col = nullptr;
public:
	void Initialize()override;
	void Update()override;
	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)override;
};

