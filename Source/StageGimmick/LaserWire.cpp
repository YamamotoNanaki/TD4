#include "LaserWire.h"

#include "Transform.h"
#include "ModelManager.h"
#include "ObjectManager.h"

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
	for (int32_t i = 0; i < poss_.size(); i++)
	{
		objects_[i]->Draw();
	}
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
	gui->DragVectorFloat3GUI(poss_, "pos", {0,0,0}, 0.25f, 0, 1000);
	gui->DragVectorFloat3GUI(rots_, "rot", {0,0,0}, 0.25f, 0, 1000);
	gui->DragVectorFloat3GUI(scales_, "scale", {0,0,0}, 0.25f, 0, 1000);

	if (oldposSize != poss_.size())
	{
		rots_.resize(poss_.size());
		scales_.resize(poss_.size());
		oldposSize = (int32_t)poss_.size();
		oldrotSize = (int32_t)rots_.size();
		oldscaleSize = (int32_t)scales_.size();
	}
	if (oldrotSize != rots_.size())
	{
		poss_.resize(rots_.size());
		scales_.resize(rots_.size());
		oldposSize = (int32_t)poss_.size();
		oldrotSize = (int32_t)rots_.size();
		oldscaleSize = (int32_t)scales_.size();
	}
	if (oldscaleSize != scales_.size())
	{
		poss_.resize(scales_.size());
		rots_.resize(scales_.size());
		oldposSize = (int32_t)poss_.size();
		oldrotSize = (int32_t)rots_.size();
		oldscaleSize = (int32_t)scales_.size();
	}
	
	//設定用のオブジェクト描画設定
	for (int32_t i = 0; i < 10; i++)
	{
		if (poss_.size() - 1 < i)
		{
			testobj_[i].DrawFlag_ = false;
		}
		else
		{
			testobj_[i].DrawFlag_ = true;
		}
	}

	for (int32_t i = 0; i < poss_.size(); i++)
	{

		//限界なら処理しない
		if (i > 10)continue;
		testobj_[i].transform_->position_ = poss_[i];
		testobj_[i].transform_->rotation_ = transform_->rotation_;
		testobj_[i].transform_->scale_ = scales_[i];
	}
	
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