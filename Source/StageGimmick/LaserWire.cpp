#include "LaserWire.h"

#include "Transform.h"
#include "ModelManager.h"
#include "ObjectManager.h"
#include "JsonManager.h"

void LaserWire::Initialize()
{
	cols_ = objectPtr_->GetComponent<IFE::Collider>();
	objects_.clear();
	for (int32_t i = 0; i < poss_.size(); i++)
	{
		IFE::Object3D* addObj=IFE::ObjectManager::Instance()->AddInitialize("wireobj", IFE::ModelManager::Instance()->GetModel("dice"));
		addObj->transform_->position_ = poss_[i];
		addObj->transform_->rotation_ = transform_->rotation_;
		addObj->transform_->scale_ = scales_[i];
		addObj->transform_->parent_ = objectPtr_->transform_;
		objects_.push_back(addObj);
	}
}

void LaserWire::Update()
{
	for (int16_t i = 0; i < poss_.size(); i++)
	{
		auto col = cols_->GetCollider(i);
		if (col == nullptr)
		{
			col = cols_->AddCollider();
		}
		col->SetOffsetPosition(poss_[i]);
		col->SetOffsetScale(scales_[i]);
	}
}

void LaserWire::Draw()
{
#ifdef EditorMode
	
#endif
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
	
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloat3GUI(&transform_->position_, "points", 0.25f, -1000, 1000);
	gui->DragVectorFloat3GUI(poss_, "pos", {0,0,0}, 0.25f, -1000, 1000);
	gui->DragVectorFloat3GUI(scales_, "scale", {0,0,0}, 0.25f, 1, 1000);
	

	if (oldposSize != poss_.size())
	{
		rots_.resize(poss_.size());
		scales_.resize(poss_.size());
		oldposSize = (int32_t)poss_.size();
		oldscaleSize = (int32_t)scales_.size();
	}
	if (oldscaleSize != scales_.size())
	{
		/*poss_.resize(scales_.size());
		rots_.resize(scales_.size());*/
		oldposSize = (int32_t)poss_.size();
		oldscaleSize = (int32_t)scales_.size();
	}
	
	
	
}

void LaserWire::OutputComponent(nlohmann::json& json)
{
	for (int32_t i = 0; i < poss_.size(); i++)
	{
		IFE::JsonManager::Instance()->OutputFloat3(json["pos"][i], poss_[i]);
		IFE::JsonManager::Instance()->OutputFloat3(json["scale"][i], scales_[i]);
	}
}
#endif

void LaserWire::LoadingComponent(nlohmann::json& json)
{
	poss_.clear();
	scales_.clear();
	int32_t count = 0;
	for (nlohmann::json& events : json["pos"])
	{
		IFE::Float3 addPos;
		addPos.x = events[0];
		addPos.y = events[1];
		addPos.z = events[2];
		poss_.push_back(addPos);
	}
	count = 0;
	for (nlohmann::json& events : json["scale"])
	{
		IFE::Float3 addScale;
		addScale.x = events[0];
		addScale.y = events[1];
		addScale.z = events[2];
		scales_.push_back(addScale);
	}
}