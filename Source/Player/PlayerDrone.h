#pragma once
#include <COmponent.h>

class PlayerDrone :public IFE::Component
{
private:

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);
};