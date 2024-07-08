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
	//ポーズ切り替え
	PoseChange();
	//ポーズ中選択
	PoseSelect();
}

void PoseMenu::Finalize()
{
}

bool PoseMenu::GetPoseFlag()
{
	return poseFlag_;
}

bool PoseMenu::GetCofigFlag()
{
	return configFlag_;
}

void PoseMenu::SetConfigFlag(bool flag)
{
	configFlag_ = flag;
}

void PoseMenu::PoseChange()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Esc) || IFE::Input::PadTrigger(IFE::PADCODE::START))
	{
		poseFlag_ = !poseFlag_;
		//画面を暗くするスプライトの描画
		IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = poseFlag_;

		//時間を止める処理
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
	if (poseFlag_ == true&& configFlag_==false)
	{
		if (IFE::Input::GetKeyTrigger(IFE::Key::UP) || oldLAnalog_ < 0.5f && IFE::Input::GetLYAnalog(range) > 0.5f)
		{
			selectNum_--;
		}
		else if (IFE::Input::GetKeyTrigger(IFE::Key::DOWN) || oldLAnalog_ > -0.5f && IFE::Input::GetLYAnalog(range) < -0.5f)
		{
			selectNum_++;
		}
		//フラグが限界を超えない処理
		selectNum_ = std::clamp(selectNum_, minSelectNum_, maxSelectNum_);

		//選択中のintの値をselectFlag_に同期させる処理
		selectFlag_ = static_cast<PoseSelectFlag>(selectNum_);
		oldLAnalog_ = IFE::Input::GetLYAnalog(range);
	}
}

void PoseMenu::Select()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A))
	{
		switch (selectFlag_)
		{
		case PoseSelectFlag::RETURNGAME:
			Reset();
			break;
		case PoseSelectFlag::RETURNTITLE:
			Reset();
			break;
		case PoseSelectFlag::CONFIG:
			configFlag_ = true;
			break;
		default:
			break;
		}
	}
}

void PoseMenu::Reset()
{
	selectFlag_ = PoseSelectFlag::RETURNTITLE;
	selectNum_ = 0;
	oldLAnalog_ = 0.0f;
}
