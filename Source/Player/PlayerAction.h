#pragma once
#include "Component.h"

class PlayerAction : public IFE::Component
{
private:

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);
};