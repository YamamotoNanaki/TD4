#pragma once
#include "Component.h"
#include "Collider.h"
#include "CollisionPrimitive.h"
#include "IFEMath.h"

class DroneKeepoutZoneObject :public IFE::Component
{
	using Component::Component;
public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;

	void ChangeActiveZone(bool flag);

public:

	

private:

	//âüÇµñﬂÇµèàóù
	void PushBack(IFE::ColliderCore* HitCol, const IFE::Vector3& reject);

	
};