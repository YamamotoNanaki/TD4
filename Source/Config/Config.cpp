#include "Config.h"
#include "Input.h"
#include"ObjectManager.h"

void Config::Initialize()
{
	pose_ = IFE::ObjectManager::Instance()->GetObjectPtr("PoseMenu")->GetComponent<PoseMenu>();
}

void Config::Update()
{
	if (oldPoseFlag_ == true && pose_->GetPoseFlag() == false)
	{
		Reset();
	}

	if (pose_->GetPoseFlag() == true)
	{
		ConfigSelect();
		ConfigChange();
	}

	oldPoseFlag_ = pose_->GetPoseFlag();
}

void Config::Finalize()
{
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
}

void Config::GageConfig(float& configValue)
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT))
	{
		configValue--;
	}
	if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT))
	{
		configValue++;
	}
	if (IFE::Input::GetLXAnalog() < 0.0f)
	{
		configValue -= IFE::Input::GetLXAnalog();
	}
	if (IFE::Input::GetLXAnalog() > 0.0f)
	{
		configValue += IFE::Input::GetLXAnalog();
	}
	configValue = std::clamp(configValue, 0.0f, 255.0f);
}

void Config::BottonConfig(bool& configValue)
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT)||oldLAnalog_.x > -0.5f && IFE::Input::GetLXAnalog(10000) < -0.5f)
	{
		configValue = false;
	}
	if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_.x < 0.5f && IFE::Input::GetLXAnalog(10000) > 0.5f)
	{
		configValue = true;
	}
}

void Config::Reset()
{
	selectNum_ = 0;
	oldLAnalog_ = 0.0f;
	configFlag_ = ConfigFlag::brightness;
}

void Config::ConfigReset()
{
	configValue_.brightness=0.0f;
	configValue_.masterValume = 0.0f;
	configValue_.BGMValume = 0.0f;
	configValue_.SEValume = 0.0f;
	configValue_.cameraReverse = false;
}