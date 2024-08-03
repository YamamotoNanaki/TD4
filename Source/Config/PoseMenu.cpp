#include "PoseMenu.h"
#include "Input.h"
#include"SpriteManager.h"
#include"ColorBuffer.h"
#include"Scene.h"
#include "Tutorial.h"
#include "ObjectManager.h"

PoseMenu::PoseMenu()
{
}

PoseMenu::~PoseMenu()
{
}

void PoseMenu::Initialize()
{
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("resume")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("resume")->GetComponent<IFE::ColorBuffer>()->SetAlpha(1.0f);
	IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.5f);
	IFE::SpriteManager::Instance()->GetSpritePtr("config")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("config")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.5f);
}

void PoseMenu::Update()
{
	//ポーズ切り替え
	if (configFlag_ == false)
	{
		PoseChange();
		if (poseFlag_ == true && configFlag_ == false)
		{
			//ポーズ中選択
			PoseSelect();
			Select();
		}
	}
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
	if (IFE::Input::GetKeyTrigger(IFE::Key::Enter) || IFE::Input::PadTrigger(IFE::PADCODE::START))
	{
		poseFlag_ = !poseFlag_;

		//画面を暗くするスプライトの描画
		IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = poseFlag_;
		IFE::SpriteManager::Instance()->GetSpritePtr("resume")->drawFlag_ = poseFlag_;
		IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->drawFlag_ = poseFlag_;
		IFE::SpriteManager::Instance()->GetSpritePtr("config")->drawFlag_ = poseFlag_;

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
	if (poseFlag_ == true && configFlag_ == false)
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

	if (IFE::Input::GetKeyTrigger(IFE::Key::Esc) || IFE::Input::PadTrigger(IFE::PADCODE::B))
	{
		Reset();
	}

	if (selectNum_ != oldSelectNum_)
	{
		switch (selectFlag_)
		{
		case PoseSelectFlag::RETURNGAME:
			IFE::SpriteManager::Instance()->GetSpritePtr("resume")->GetComponent<IFE::ColorBuffer>()->SetColor({1.0f,1.0f ,1.0f ,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->GetComponent<IFE::ColorBuffer>()->SetColor({0.25f,0.25f ,0.25f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("config")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f,1.0f });
			break;
		case PoseSelectFlag::RETURNTITLE:
			IFE::SpriteManager::Instance()->GetSpritePtr("resume")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("config")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f,1.0f });
			break;
		case PoseSelectFlag::CONFIG:
			IFE::SpriteManager::Instance()->GetSpritePtr("resume")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("config")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
			break;
		default:
			break;
		}
	}
	oldSelectNum_ = selectNum_;
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
			IFE::Scene::Instance()->SetNextScene("TITLE");
			break;
		case PoseSelectFlag::CONFIG:
			IFE::SpriteManager::Instance()->GetSpritePtr("resume")->drawFlag_ = false;
			IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->drawFlag_ = false;
			IFE::SpriteManager::Instance()->GetSpritePtr("config")->drawFlag_ = false;
			configFlag_ = true;
			break;
		default:
			break;
		}
	}
}

void PoseMenu::Reset()
{
	selectFlag_ = PoseSelectFlag::RETURNGAME;
	selectNum_ = 0;
	oldLAnalog_ = 0.0f;
	poseFlag_ = false;
	configFlag_ = false;
	IFE::IFETime::sTimeScale_ = 1.0f;

	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = poseFlag_;
	IFE::SpriteManager::Instance()->GetSpritePtr("resume")->drawFlag_ = poseFlag_;
	IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->drawFlag_ = poseFlag_;
	IFE::SpriteManager::Instance()->GetSpritePtr("config")->drawFlag_ = poseFlag_;
}
