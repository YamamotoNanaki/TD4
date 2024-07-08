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
	if (!parentBone_)
	{
		SetTransform();
		if (!parentBone_)
		{
			transform_->SetAttach3DParent(nullptr);
			parentBone_ = nullptr;
		}
	}
}

void IFE::Attach3DModel::SetTransform()
{
	auto ptr = ObjectManager::Instance()->GetObjectPtr(parentName_);
	if (!ptr)return;
	auto model_ = dynamic_cast<FBXModel*>(ptr->GetModel());
	if (!model_ || boneName_ == "")return;
	parentBone_ = model_->GetBone(boneName_);
	transform_->SetAttach3DParent(parentBone_);
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
	if (im->NewTreeNode(U8("ボーン変更")))
	{
		im->InputTextGUI("boneName", bone);
		if (im->ButtonGUI("Change"))
		{
			boneName_ = bone;
			SetTransform();
		}
		im->EndTreeNode();
	}
	if (im->NewTreeNode(U8("親オブジェクト変更")))
	{
		parent = ObjectManager::Instance()->GetObjectNameGUI();
		if (im->ButtonGUI("Change"))
		{
			parentName_ = parent;
			SetTransform();
		}
		im->EndTreeNode();
	}
	if (parentBone_ && im->NewTreeNode(U8("確認用")))
	{
		im->TextMatrixGUI(parentBone_->finalMatrix);
		im->EndTreeNode();
	}
}

void IFE::Attach3DModel::OutputComponent(nlohmann::json& json)
{
	json["boneName"] = boneName_;
	json["parentName"] = parentName_;
}
#endif
