#pragma once
#include"Component.h"
#include"IFMath.h"

class Player:public IFE::Component
{
private:

	


public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();
};