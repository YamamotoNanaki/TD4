#include "PoseMenu.h"
#include "Input.h"
#include"SpriteManager.h"

PoseMenu::PoseMenu()
{
}

PoseMenu::~PoseMenu()
{
}

void PoseMenu::Initialize()
{
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = false;
}

void PoseMenu::Update()
{
	//�|�[�Y�؂�ւ�
	PoseChange();
	//�|�[�Y���I��
	PoseSelect();
}

void PoseMenu::Finalize()
{
}

bool PoseMenu::GetPoseFlag()
{
	return poseFlag_;
}

void PoseMenu::PoseChange()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Esc) || IFE::Input::PadTrigger(IFE::PADCODE::START))
	{
		poseFlag_ = !poseFlag_;
		//��ʂ��Â�����X�v���C�g�̕`��
		IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = poseFlag_;

		//���Ԃ��~�߂鏈��
		if (poseFlag_ == true)
		{
			IFE::IFETime::sTimeScale_ = 0.0f;
		}
		else
		{
			IFE::IFETime::sTimeScale_ = 1.0f;
		}
	}
}

void PoseMenu::PoseSelect()
{
	int32_t range = 10000;
	if (poseFlag_ == true)
	{
		if (IFE::Input::GetKeyTrigger(IFE::Key::UP) || oldLAnalog_ < 0.5f && IFE::Input::GetLYAnalog(range) > 0.5f)
		{
			selectNum_--;
		}
		else if (IFE::Input::GetKeyTrigger(IFE::Key::DOWN) || oldLAnalog_ > -0.5f && IFE::Input::GetLYAnalog(range) < -0.5f)
		{
			selectNum_++;
		}
		//�t���O�����E�𒴂��Ȃ�����
		selectNum_ = std::clamp(selectNum_, minSelectNum_, maxSelectNum_);
	}
	else
	{
		selectFlag_ = PoseSelectFlag::RETURNTITLE;
		selectNum_ = 0;
	}

	//�I�𒆂�int�̒l��selectFlag_�ɓ��������鏈��
	selectFlag_ = static_cast<PoseSelectFlag>(selectNum_);
	oldLAnalog_ = IFE::Input::GetLYAnalog(range);
}