#include "DroneKeepoutZoneObject.h"

void DroneKeepoutZoneObject::Initialize()
{

}

void DroneKeepoutZoneObject::Update()
{

}

void DroneKeepoutZoneObject::Draw()
{

}

void DroneKeepoutZoneObject::Finalize()
{

}

void DroneKeepoutZoneObject::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	myCollider;
	hitCollider;
}

#ifdef EditorMode
#include "ImguiManager.h"
void DroneKeepoutZoneObject::ComponentDebugGUI() 
{

}

void DroneKeepoutZoneObject::OutputComponent(nlohmann::json& json)
{
	json;
}

#endif

void DroneKeepoutZoneObject::LoadingComponent(nlohmann::json& json)
{
	json;
}