#include "Config.h"
#include "Input.h"
#include"ObjectManager.h"
#include "ImguiManager.h"
#include"ColorBuffer.h"
#include"SpriteManager.h"

void Config::Initialize()
{
	pose_ = IFE::ObjectManager::Instance()->GetObjectPtr("PoseMenu")->GetComponent<PoseMenu>();

	IFE::SpriteManager::Instance()->GetSpritePtr("brightnessAdjustment")->order_ = 254;
}

void Config::Update()
{
	if (oldConfigFlag_ == false && pose_->GetCofigFlag() == true)
	{
		IFE::SpriteManager::Instance()->GetSpritePtr("brightness")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("brightnessMemoly")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("brightnessThumb")->drawFlag_ = true;
	}

	if (pose_->GetCofigFlag() == true)
	{
		if (IFE::Input::GetKeyTrigger(IFE::Key::Esc) || IFE::Input::PadTrigger(IFE::PADCODE::B))
		{
			IFE::SpriteManager::Instance()->GetSpritePtr("resume")->drawFlag_ = true;
			IFE::SpriteManager::Instance()->GetSpritePtr("returnTitle")->drawFlag_ = true;
			IFE::SpriteManager::Instance()->GetSpritePtr("config")->drawFlag_ = true;
			Reset();
		}
		if (IFE::Input::GetKeyTrigger(IFE::Key::Enter) || IFE::Input::PadTrigger(IFE::PADCODE::START))
		{
			pose_->Reset();
			Reset();
		}

		if (pose_->GetCofigFlag() == true)
		{
			ConfigSelect();
			ConfigChange();
		}

		IFE::ImguiManager::Instance()->TextIntGUI(selectNum_);
		IFE::ImguiManager::Instance()->TextFloatGUI(configValue_.brightness);
	}
	oldConfigFlag_ = pose_->GetCofigFlag();
}

void Config::Finalize()
{
}

const ConfigValue& Config::GetConfigValue()
{
	return configValue_;
}

void Config::ConfigSelect()
{
	int32_t range = 10000;

	if (IFE::Input::GetKeyTrigger(IFE::Key::UP) || oldLAnalog_.y < 0.5f && IFE::Input::GetLYAnalog(range) > 0.5f)
	{
		selectNum_--;
	}
	else if (IFE::Input::GetKeyTrigger(IFE::Key::DOWN) || oldLAnalog_.y > -0.5f && IFE::Input::GetLYAnalog(range) < -0.5f)
	{
		selectNum_++;
	}
	//フラグが限界を超えない処理
	selectNum_ = std::clamp(selectNum_, minSelectNum_, maxSelectNum_);

	//選択中のintの値をselectFlag_に同期させる処理
	configFlag_ = static_cast<ConfigFlag>(selectNum_);

	oldLAnalog_.x = IFE::Input::GetLXAnalog(range);
	oldLAnalog_.y = IFE::Input::GetLYAnalog(range);
}

void Config::ConfigChange()
{
	switch (configFlag_)
	{
	case ConfigFlag::brightness:
		GageConfig(configValue_.brightness);
		configValue_.brightness = min(configValue_.brightness, 0.9f);
		IFE::SpriteManager::Instance()->GetSpritePtr("brightnessAdjustment")->GetComponent<IFE::ColorBuffer>()->SetAlpha(configValue_.brightness);
		IFE::SpriteManager::Instance()->GetSpritePtr("brightnessThumb")->transform_->transform2D_->position2D_.x = 840.0f + configValue_.brightness * 800.0f;
		break;
	case ConfigFlag::masterValume:
		GageConfig(configValue_.masterValume);
		break;
	case ConfigFlag::BGMValume:
		GageConfig(configValue_.BGMValume);
		break;
	case ConfigFlag::SEValume:
		GageConfig(configValue_.SEValume);
		break;
	case ConfigFlag::cameraReverse:
		BottonConfig(configValue_.cameraReverse);
		break;
	default:
		break;
	}

	SelectNow(ConfigFlag::brightness, "brightnessThumb");
}

void Config::GageConfig(float& configValue)
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT))
	{
		configValue -= 1 / 255.0f;
	}
	if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT))
	{
		configValue += 1 / 255.0f;
	}

	configValue += IFE::Input::GetLXAnalog(10000) / 255.0f;

	configValue = std::clamp(configValue, 0.0f, 1.0f);
}

void Config::BottonConfig(bool& configValue)
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT) || oldLAnalog_.x > -0.5f && IFE::Input::GetLXAnalog(10000) < -0.5f)
	{
		configValue = false;
	}
	if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_.x < 0.5f && IFE::Input::GetLXAnalog(10000) > 0.5f)
	{
		configValue = true;
	}
}

void Config::SelectNow(ConfigFlag flag, std::string name)
{
	if (configFlag_ == flag)
	{
		IFE::SpriteManager::Instance()->GetSpritePtr(name)->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	else
	{
		IFE::SpriteManager::Instance()->GetSpritePtr(name)->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f,0.25f,1.0f });
	}
}

void Config::Reset()
{
	selectNum_ = 0;
	oldLAnalog_ = 0.0f;
	configFlag_ = ConfigFlag::brightness;
	oldConfigFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("brightness")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("brightnessMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("brightnessThumb")->drawFlag_ = false;

	pose_->SetConfigFlag(false);
}

void Config::ConfigReset()
{
	configValue_.brightness = 0.0f;
	configValue_.masterValume = 0.5f;
	configValue_.BGMValume = 0.5f;
	configValue_.SEValume = 0.5f;
	configValue_.cameraReverse = false;
}