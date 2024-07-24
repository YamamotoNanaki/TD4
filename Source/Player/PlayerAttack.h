#pragma once
#include"Component.h"

class PlayerAttack :public IFE::Component
{
	using Component::Component;
private:

	std::string name_ = "";
	bool isAttack_ = false;
	bool isBackAttack_ = false;
	bool attackFlag_ = false;

public:

	/// <summary>
	/// èâä˙âª
	/// </summary>
	void Initialize();

	/// <summary>
	/// çXêV
	/// </summary>
	void Update();

	/// <summary>
	/// èIóπ
	/// </summary>
	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)override;

	void SetName(std::string name);

	std::string GetName();

	void SetIsAttack(bool flag);

	void SetAttackFlag(bool atk);

	const bool GetIsAttack();

	bool GetIsBackAttack();
	void SetIsBackAttack(bool flag);
};