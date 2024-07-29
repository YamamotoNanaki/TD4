#pragma once
#include "Component.h"
#include "IFEEffekseer.h"
#include "Transform.h"

class Blood : public IFE::Component
{
	IFE::IFEEffekseer* efk_;
	IFE::Float3 pos;
	IFE::Float3 rot;
	IFE::Transform* tra;

public:
	void Initialize();
	void Update();
};

