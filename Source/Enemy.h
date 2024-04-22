#pragma once
#include"Component.h"
#include"IFEMath.h"

class Enemy :public IFE::Component
{
private:




public:

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize();

	/// <summary>
	/// XV
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

	/// <summary>
	/// I—¹
	/// </summary>
	void Finalize();
};