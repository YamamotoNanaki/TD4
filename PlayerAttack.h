#pragma once
#include"Component.h"

class PlayerAttack :public IFE::Component
{
	using Component::Component;
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
	/// I—¹
	/// </summary>
	void Finalize();


};