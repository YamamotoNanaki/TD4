#pragma once
#include "Component.h"

class PlayerActionCamera:public IFE::Component
{
private:



public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore collider);
};