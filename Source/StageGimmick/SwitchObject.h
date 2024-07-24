#pragma once
#include "Component.h"
#include "Object3D.h"
#include "Collider.h"
#include "IEvent.h"

class SwitchObject :public IFE::Component
{
public:

	//現在当たっているか
	bool isHit_ = false;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

	bool GetIsHit() { return isHit_; };

	void SetIsActive(bool flag);

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;


private:

	//当たり判定のtransform
	IFE::Float3 scale_ = { 1,1,1 };


	IFE::Collider* cols_;

	float countHitTimer_ = 0;
	float hitMaxTime_ = 0;

	std::unique_ptr<IFE::IEvent> event_;

	//いったんイベントの種類を記録するために用意
	int32_t eventType_ = EventType::damage;

	bool isEventStart = false;

	//スイッチが起動したか
	bool isSwitchObject_ = false;

	//ギミックが動いているか
	bool isActive_ = true;

	bool testflag_ = true;

private:

};