#pragma once
#include "Component.h"

class StageCollideManageer : public IFE::Component
{
	std::list<IFE::Object3D*>stages;
public:
	void Initialize()override;
	void Update()override;
};

