#include "LaserWire.h"
#include "Collider.h"
#include "Object3D.h"
#include"Transform.h"

void LaserWire::Initialize()
{

}

void LaserWire::Update()
{

}

void LaserWire::Draw()
{

}

void LaserWire::Finalize()
{

}

void LaserWire::OnColliderHit(IFE::ColliderCore collider)
{
	isHit_ = true;
}

#ifdef EditorMode
#include "ImguiManager.h"
void LaserWire::ComponentDebugGUI()
{
	IFE::Vector3 posBuff = transform_->position_;
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloat3GUI(&posBuff, "points", 0.25f,-1000,1000);

	transform_->position_ = posBuff;

}

void LaserWire::OutputComponent(nlohmann::json& json)
{
	json;
}
#endif

void LaserWire::LoadingComponent(nlohmann::json& json)
{
	json;
}

