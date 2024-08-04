#include "Config.h"
#include "Input.h"
#include"ObjectManager.h"
#include "ImguiManager.h"
#include"ColorBuffer.h"
#include"SpriteManager.h"
#include"Sound.h"

void Config::Initialize()
{
	pose_ = IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->GetComponent<PoseMenu>();

	IFE::SpriteManager::Instance()->GetSpritePtr("brightness")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("brightnessMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("brightnessThumb")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("volume")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("masterVolume")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("masterVolumeMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("masterVolumeThumb")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolume")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolumeMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolumeThumb")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("SEVolume")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("SEVolumeMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("SEVolumeThumb")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("inversionCamera")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraON")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraOFF")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("brightnessAdjustment")->order_ = 254;
}

void Config::Update()
{
	if (oldConfigFlag_ == false && pose_->GetCofigFlag() == true)
	{
		IFE::SpriteManager::Instance()->GetSpritePtr("brightness")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("brightnessMemoly")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("brightnessThumb")->drawFlag_ = true;

		IFE::SpriteManager::Instance()->GetSpritePtr("volume")->drawFlag_ = true;

		IFE::SpriteManager::Instance()->GetSpritePtr("masterVolume")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("masterVolumeMemoly")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("masterVolumeThumb")->drawFlag_ = true;

		IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolume")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolumeMemoly")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolumeThumb")->drawFlag_ = true;

		IFE::SpriteManager::Instance()->GetSpritePtr("SEVolume")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("SEVolumeMemoly")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("SEVolumeThumb")->drawFlag_ = true;

		IFE::SpriteManager::Instance()->GetSpritePtr("inversionCamera")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraON")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraOFF")->drawFlag_ = true;
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

	if (IFE::Input::GetKeyTrigger(IFE::Key::UP) || oldLAnalog_.y < 0.5f && IFE::Input::GetLYAnalog(range) > 0.5f || IFE::Input::PadTrigger(IFE::PADCODE::UP))
	{
		selectNum_--;
	}
	else if (IFE::Input::GetKeyTrigger(IFE::Key::DOWN) || oldLAnalog_.y > -0.5f && IFE::Input::GetLYAnalog(range) < -0.5f || IFE::Input::PadTrigger(IFE::PADCODE::DOWN))
	{
		selectNum_++;
	}
	//フラグが限界を超えない処理
	selectNum_ = std::clamp(selectNum_, minSelectNum_, maxSelectNum_);

	//選択中のintの値をselectFlag_に同期させる処理
	configFlag_ = static_cast<ConfigFlag>(selectNum_);
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
		IFE::Sound::Instance()->SetMasterVolume(configValue_.masterValume);
		IFE::Sound::Instance()->AllSoundVolumeUpdate();
		IFE::SpriteManager::Instance()->GetSpritePtr("masterVolumeThumb")->transform_->transform2D_->position2D_.x = 840.0f + configValue_.masterValume * 720.0f;
		break;
	case ConfigFlag::BGMValume:
		GageConfig(configValue_.BGMValume);
		IFE::Sound::Instance()->SetBGMVolume(configValue_.BGMValume);
		IFE::Sound::Instance()->AllSoundVolumeUpdate();
		IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolumeThumb")->transform_->transform2D_->position2D_.x = 840.0f + configValue_.BGMValume * 720.0f;
		break;
	case ConfigFlag::SEValume:
		GageConfig(configValue_.SEValume);
		IFE::Sound::Instance()->SetSEVolume(configValue_.SEValume);
		IFE::Sound::Instance()->AllSoundVolumeUpdate();
		IFE::SpriteManager::Instance()->GetSpritePtr("SEVolumeThumb")->transform_->transform2D_->position2D_.x = 840.0f + configValue_.SEValume * 720.0f;
		break;
	case ConfigFlag::cameraReverse:
		BottonConfig(configValue_.cameraReverse);
		break;
	default:
		break;
	}

	SelectNow(ConfigFlag::brightness, "brightnessThumb");
	SelectNow(ConfigFlag::masterValume, "masterVolumeThumb");
	SelectNow(ConfigFlag::BGMValume, "BGMVolumeThumb");
	SelectNow(ConfigFlag::SEValume, "SEVolumeThumb");

	if (configFlag_ == ConfigFlag::cameraReverse)
	{
		if (configValue_.cameraReverse == false)
		{
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraON")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f,0.25f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraOFF")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
		else
		{
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraON")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraOFF")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f,0.25f,1.0f });
		}
	}
	else
	{
		if (configValue_.cameraReverse == false)
		{
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraON")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.125f,0.125f,0.125f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraOFF")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.5f,0.5f,0.5f,1.0f });
		}
		else
		{
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraON")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.5f,0.5f,0.5f,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraOFF")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.125f,0.125f,0.125f,1.0f });
		}
	}
	oldLAnalog_.x = IFE::Input::GetLXAnalog(10000);
	oldLAnalog_.y = IFE::Input::GetLYAnalog(10000);
}

void Config::GageConfig(float& configValue)
{
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT) || IFE::Input::PadPush(IFE::PADCODE::LEFT))
	{
		configValue -= 1 / 255.0f;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT) || IFE::Input::PadPush(IFE::PADCODE::RIGHT))
	{
		configValue += 1 / 255.0f;
	}

	configValue += IFE::Input::GetLXAnalog(10000) / 255.0f;

	configValue = std::clamp(configValue, 0.0f, 1.0f);
}

void Config::BottonConfig(bool& configValue)
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT) || oldLAnalog_.x > -0.5f && IFE::Input::GetLXAnalog(10000) < -0.5f || IFE::Input::PadTrigger(IFE::PADCODE::LEFT))
	{
		configValue = false;
	}
	if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_.x < 0.5f && IFE::Input::GetLXAnalog(10000) >0.5f || IFE::Input::PadTrigger(IFE::PADCODE::RIGHT))
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

	IFE::SpriteManager::Instance()->GetSpritePtr("volume")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("masterVolume")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("masterVolumeMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("masterVolumeThumb")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolume")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolumeMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("BGMVolumeThumb")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("SEVolume")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("SEVolumeMemoly")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("SEVolumeThumb")->drawFlag_ = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("inversionCamera")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraON")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("inversionCameraOFF")->drawFlag_ = false;

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