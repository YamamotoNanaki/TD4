#pragma once
#include "Component.h"
#include "IFEMath.h"

class LaserWire:public IFE::Component
{
public:

	//���ݓ������Ă��邩
	bool isHit_ = false;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore collider);


#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;


private:

	IFE::Vector3 pos_;

};

