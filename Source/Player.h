#pragma once
#include"Component.h"
#include"IFEMath.h"

class Player:public IFE::Component
{
private:

	


public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::Collider collider);
};