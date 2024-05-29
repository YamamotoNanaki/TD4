#pragma once
#include "Component.h"

class StageCollideManageer : public IFE::Component
{
public:
	void Initialize()override;
	void Update()override;
	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)override;
};

