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
		//フラグが限界を超えない処理
		selectNum_ = std::clamp(selectNum_, minSelectNum_, maxSelectNum_);
	}
	else
	{
		selectFlag_ = PoseSelectFlag::RETURNTITLE;
		selectNum_ = 0;
	}

	//選択中のintの値をselectFlag_に同期させる処理
	selectFlag_ = static_cast<PoseSelectFlag>(selectNum_);
	oldLAnalog_ = IFE::Input::GetLYAnalog(range);
}