#include "Animator.h"
#include "IFETime.h"
#include "Object3D.h"
#include "GraphicsPipelineManager.h"

void IFE::Animator::Initialize()
{
	skinBuffer_ = std::make_unique<ConstBuffer<ConstBufferDataSkin>>();
	constMapSkin_ = skinBuffer_->GetCBMapObject();
	objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dAnim");
	model_ = dynamic_cast<FBXModel*>(objectPtr_->model_);
}

void IFE::Animator::DebugInitialize()
{
	skinBuffer_ = std::make_unique<ConstBuffer<ConstBufferDataSkin>>();
	constMapSkin_ = skinBuffer_->GetCBMapObject();
	objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dAnim");
	model_ = dynamic_cast<FBXModel*>(objectPtr_->model_);
}

void IFE::Animator::Update()
{
	if (!animFlag_)return;
	if (animNum_ > model_->animations_.size())animNum_ = oldAnimNum_;
	if (oldAnimNum_ != animNum_)animTimer_ = 0;
	animTimer_ += animSpeed_ * IFETime::sDeltaTime_;
	animEnd_ = false;
	if (animTimer_ >= model_->animations_[animNum_].endTime)
	{
		if (loop_)
		{
			animTimer_ -= (float)model_->animations_[animNum_].endTime;
		}
		else
		{
			animEnd_ = true;
			animTimer_ = (float)model_->animations_[animNum_].endTime - FLT_EPSILON;
		}
	}
	oldAnimNum_ = animNum_;
}

void IFE::Animator::Draw()
{
	model_->BoneTransform(animTimer_, animNum_);
	for (int i = 0; i < model_->bones_.size(); i++)
	{
		constMapSkin_->bones[i] = model_->bones_[i].finalMatrix;
	}
	skinBuffer_->SetConstBuffView(6);
}

float IFE::Animator::GetEndTime()
{
	return model_->animations_[animNum_].endTime;
}

IFE::Animator::~Animator()
{
	objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("3dNormal");
}

void IFE::Animator::SetAnimation(std::string animName)
{
	uint8_t i = 0;
	for (auto& anim : model_->animations_)
	{
		if (anim.name == animName)
		{
			animNum_ = i;
			break;
		}
		i++;
	}
}

std::string IFE::Animator::GetAnimation()
{
	return model_->animations_[animNum_].name;
}

void IFE::Animator::ModelUpdate()
{
	model_ = dynamic_cast<FBXModel*>(objectPtr_->model_);
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
void IFE::Animator::ComponentDebugGUI()
{
	ImguiManager* imgui = ImguiManager::Instance();
	int32_t num = animNum_;
	imgui->DragIntGUI(&num, "Set animation");
	animNum_ = (uint8_t)num;
	if (imgui->NewTreeNode("Show all animation names"))
	{
		for (uint8_t i = 0; i < model_->animations_.size(); i++)
		{
			std::string text = std::to_string(i) + model_->animations_[i].name;
			imgui->TextGUI(text);
		}
		imgui->EndTreeNode();
	}
}
#endif
