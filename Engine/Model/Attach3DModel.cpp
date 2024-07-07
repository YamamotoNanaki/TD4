#include "Attach3DModel.h"
#include "Object3D.h"
#include "ObjectManager.h"
#include "Transform.h"

void IFE::Attach3DModel::Initialize()
{
	SetTransform();
}

void IFE::Attach3DModel::Update()
{
	if (!parentTransform_)
	{
		SetTransform();
		if (!parentTransform_)
		{
			transform_->SetAttach3DParent(nullptr);
			parentTransform_ = nullptr;
		}
	}
}

void IFE::Attach3DModel::SetTransform()
{
	auto ptr = ObjectManager::Instance()->GetObjectPtr(parentName_);
	if (!ptr)return;
	auto model_ = dynamic_cast<FBXModel*>(ptr->GetModel());
	if (!model_ || boneName_ == "")return;
	parentTransform_ = model_->GetBoneTransform(boneName_);
	transform_->SetAttach3DParent(parentTransform_);
	transform_->parent_ = ptr->transform_;
}

void IFE::Attach3DModel::LoadingComponent(nlohmann::json& json)
{
	boneName_ = json["boneName"];
	parentName_ = json["parentName"];
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::Attach3DModel::ComponentDebugGUI()
{
	auto im = ImguiManager::Instance();
	static std::string bone;
	static std::string parent;
	im->TextGUI(U8("ボーン : " + boneName_));
	im->TextGUI(U8("親オブジェクト : " + parentName_));
	if (im->NewTreeNode(U8("変更")))
	{
		im->InputTextGUI("boneName", bone);
		parent = ObjectManager::Instance()->GetObjectNameGUI();
		if (im->ButtonGUI("Change"))
		{
			boneName_ = bone;
			parentName_ = parent;
			SetTransform();
		}
		im->EndTreeNode();
	}
}

void IFE::Attach3DModel::OutputComponent(nlohmann::json& json)
{
	json["boneName"] = boneName_;
	json["parentName"] = parentName_;
}
#endif
