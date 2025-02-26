#include "Animator.h"
#include "IFETime.h"
#include "Object3D.h"
#include "GraphicsPipelineManager.h"

void IFE::Animator::Initialize()
{
	skinBuffer_ = std::make_unique<ConstBuffer<ConstBufferDataSkin>>();
	constMapSkin_ = skinBuffer_->GetCBMapObject();
	objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("ModelAnim_DEFALUT");
	model_ = dynamic_cast<FBXModel*>(objectPtr_->model_);
	animMats_.resize(model_->bones_.size());
	for (int i = 0; i < animMats_.size(); i++)
	{
		animMats_[i].mat = model_->bones_[i].finalMatrix;
		animMats_[i].name = model_->bones_[i].name;
		if(model_->bones_[i].parent)animMats_[i].parentName = model_->bones_[i].parent->name;
	}
	for (size_t i = 0; i < animMats_.size(); i++)
	{
		if (animMats_[i].parentName != "")
		{
			for (size_t j = 0; j < animMats_.size(); j++)
			{
				if (animMats_[i].parentName == animMats_[j].name)
				{
					animMats_[i].parent = &animMats_[j];
					break;
				}
			}
		}
	}
}

void IFE::Animator::DebugInitialize()
{
	Initialize();
}

void IFE::Animator::Update()
{
	if (!animFlag_)return;
	if (animNum_ > model_->animations_.size())animNum_ = oldAnimNum_;
	if (oldAnimNum_ != animNum_)
	{
		if (interpolation_)
		{
			interpolationAnimNum_ = oldAnimNum_;
			lerpTimer_ = 0;
			oldAnimTimer_ = animTimer_;
			oldLoop_ = loop_;
			oldAnimSpeed_ = animSpeed_;
		}
		animTimer_ = 0;
	}
	if (interpolation_)
	{
		lerpTimer_ += IFETime::sDeltaTime_;
		//oldAnimTimer_ += oldAnimSpeed_ * IFETime::sDeltaTime_;
		//if (oldAnimTimer_ >= model_->animations_[interpolationAnimNum_].endTime)
		//{
		//	if (oldLoop_)
		//	{
		//		oldAnimTimer_ -= (float)model_->animations_[interpolationAnimNum_].endTime;
		//	}
		//	else
		//	{
		//		oldAnimTimer_ = (float)model_->animations_[interpolationAnimNum_].endTime - FLT_EPSILON;
		//	}
		//}
		if (lerpTimer_ >= interpolationMaxTimer_)
		{
			interpolation_ = false;
		}
	}
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
	model_->BoneTransform(animTimer_, animNum_, interpolation_, oldAnimTimer_, interpolationAnimNum_, lerpTimer_ / interpolationMaxTimer_);
	for (int i = 0; i < model_->bones_.size(); i++)
	{
		constMapSkin_->bones[i] = animMats_[i].mat = model_->bones_[i].finalMatrix;
	}
	skinBuffer_->SetConstBuffView(6);
}

float IFE::Animator::GetEndTime()
{
	return model_->animations_[animNum_].endTime;
}

void IFE::Animator::SetAnimTime(float animTime_)
{
	animTimer_ = animTime_;
}

IFE::Animator::AnimMat* IFE::Animator::GetBone(std::string name)
{
	for (auto& bone : animMats_)
	{
		if (bone.name == name)
		{
			return &bone;
		}
	}
	return nullptr;
}

IFE::Animator::~Animator()
{
	objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("Model_DEFALUT");
}

void IFE::Animator::SetAnimation(std::string animName, bool interpolation, float interpolationMaxTimer)
{
	uint8_t i = 0;
	for (auto& anim : model_->animations_)
	{
		if (anim.name == animName)
		{
			animNum_ = i;
			interpolation_ = interpolation;
			interpolationMaxTimer_ = interpolationMaxTimer;
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
	if (imgui->NewTreeNode(U8("デバッグ用パラメータ")))
	{
		imgui->TextGUI(U8("タイマー : ") + std::to_string(animTimer_));
		imgui->DragFloatGUI(&animSpeed_, U8("スピード"));
		imgui->EndTreeNode();
	}
}
#endif
