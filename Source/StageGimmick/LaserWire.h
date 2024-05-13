#pragma once
#include "Component.h"

class LaserWire:public IFE::Component
{
public:

	//åªç›ìñÇΩÇ¡ÇƒÇ¢ÇÈÇ©
	bool isHit_ = false;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore collider);

private:

	

};

