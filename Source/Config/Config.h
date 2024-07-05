#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"

class Config :public IFE::Component
{
	void Initialize();

	void Update();

	/// <summary>
	/// èIóπ
	/// </summary>
	void Finalize();
};

