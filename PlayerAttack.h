#pragma once
#include"Component.h"

class PlayerAttack :public IFE::Component
{
	using Component::Component;
private:


public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();


};