#pragma once
#include"Component.h"

class PlayerAttack :public IFE::Component
{
	using Component::Component;
private:

	std::string name_ = "";
	bool isAttack_ = false;

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

	void SetName(std::string name);

	std::string GetName();

	void SetIsAttack(bool flag);

	const bool GetIsAttack();
};