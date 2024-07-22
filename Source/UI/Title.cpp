#include "Title.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"
#include "Over.h"
#include"Transform.h"
#include"Ease.h"

using namespace IFE;

void Title::Initialize()
{
	camera_ = IFE::CameraManager::Instance()->GetCamera("titleCamera");
	camera_->transform_->eye_ = { 0.0f,5.0f,-120.0f };
	camera_->transform_->target_ = { 0.0f,20.0f,20.0f };
	cameraAngle_ = static_cast<float>(-PI) / 2;
	IFE::CameraManager::Instance()->SetActiveCamera("titleCamera");

	IFE::Sound::Instance()->LoadWave("title");
	IFE::Sound::Instance()->SetVolume("title", 50);
	isNext = false;
}

void Title::Update()
{
	switch (titleSelectFlag_)
	{
	case TitleSelect::TITLE:
		CameraRot();
		if (animationFlag_ == false)
		{
			if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
			{
				beforeEaseAngle_ = cameraAngle_;
				animationFlag_ = true;
			}
		}
		else
		{
			ToSelectAnimation();
		}
		break;
	case TitleSelect::SELECT:
		CameraRot();
		if (animationFlag_ == false)
		{
			if (Input::PadTrigger(PADCODE::B) || Input::GetKeyTrigger(Key::Esc))
			{
				animationFlag_ = true;
			}
		}

		if (animationFlag_ == true)
		{
			BackTitleAnimation();
		}

		Select();
		break;
	case TitleSelect::SELECT2:
		SelectCheck();
		break;
	default:
		break;
	}
	ImGUI();
}

void Title::CameraRot()
{
	if (animationFlag_ == false&& titleSelectFlag_==TitleSelect::TITLE)
	{
		cameraAngle_ += 0.1f * IFE::IFETime::sDeltaTime_;

		if (cameraAngle_ >= 2*PI)
		{
			cameraAngle_ = 0.0f;
		}
	}

	camera_->transform_->eye_ =
	{
		CircularMotion({0.0f,20.0f},distanse_,cameraAngle_).x,
		camera_->transform_->eye_.y,
		CircularMotion({0.0f,20.0f},distanse_,cameraAngle_).y
	};
}

void Title::ToSelectAnimation()
{
	distanse_ = IFE::EaseInOutQuart(animationTime_, 120.0f, 0.1f, maxAnimationTime_);
	camera_->transform_->eye_.y = IFE::EaseInOutBack(animationTime_, 5.0f, 120.0f, maxAnimationTime_);
	if (beforeEaseAngle_ >= PI / 2)
	{
		cameraAngle_ = IFE::EaseInOutQuart(animationTime_, beforeEaseAngle_, static_cast<float>(PI) * 3 / 2, maxAnimationTime_);
	}
	else
	{
		cameraAngle_ = IFE::EaseInOutQuart(animationTime_, beforeEaseAngle_, static_cast<float>(-PI) / 2, maxAnimationTime_);
	}

	if (animationTime_ > maxAnimationTime_)
	{
		animationFlag_ = false;
		animationTime_ = 0.0f;
		cameraAngle_ = static_cast<float>(-PI) / 2;
		titleSelectFlag_ = TitleSelect::SELECT;
	}
	else
	{
		animationTime_ += IFE::IFETime::sDeltaTime_;
	}
}

void Title::BackTitleAnimation()
{
	distanse_ = IFE::EaseInOutQuart(animationTime_, 0.1f, 120.0f, maxAnimationTime_);
	camera_->transform_->eye_.y = IFE::EaseInOutQuart(animationTime_, 120.0f, 5.0f, maxAnimationTime_);

	if (animationTime_ > maxAnimationTime_)
	{
		animationFlag_ = false;
		animationTime_ = 0.0f;
		cameraAngle_ = static_cast<float>(-PI) / 2;
		titleSelectFlag_ = TitleSelect::TITLE;
	}
	else
	{
		animationTime_ += IFE::IFETime::sDeltaTime_;
	}
}

void Title::Select()
{
	int32_t range = 10000;
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT) || oldLAnalog_ > -0.5f && IFE::Input::GetLXAnalog(range) <-0.5f)
	{
		stageNum_--;
	}
	else if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_ <0.5f && IFE::Input::GetLXAnalog(range) >0.5f)
	{
		stageNum_++;
	}

	//ÉtÉâÉOÇ™å¿äEÇí¥Ç¶Ç»Ç¢èàóù
	stageNum_ = std::clamp(stageNum_, minStageNum_, maxStageNum_);

	oldLAnalog_ = IFE::Input::GetLXAnalog(range);

	if (Input::PadTrigger(PADCODE::A) || Input::GetKeyTrigger(Key::Space))
	{
		oldLAnalog_ = 0.0f;
		titleSelectFlag_ = TitleSelect::SELECT2;
	}
}

void Title::SelectCheck()
{
	int32_t range = 10000;
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT) || oldLAnalog_ < 0.5f && IFE::Input::GetLXAnalog(range) > 0.5f)
	{
		checkFlag_ = true;
	}
	else if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_ > -0.5f && IFE::Input::GetLXAnalog(range) < -0.5f)
	{
		checkFlag_ = false;
	}

	if (Input::PadTrigger(PADCODE::B) || Input::GetKeyTrigger(Key::Esc))
	{
		oldLAnalog_ = 0.0f;
		titleSelectFlag_ = TitleSelect::SELECT;
	}

	if (Input::PadTrigger(PADCODE::A) || Input::GetKeyTrigger(Key::Space))
	{
		if (checkFlag_ == false)
		{
			oldLAnalog_ = 0.0f;
			titleSelectFlag_ = TitleSelect::SELECT;
		}
		else
		{
			if (!isNext) {
				IFE::Sound::Instance()->SoundPlay("title", false, true);
			}
			isNext = true;

			Scene::Instance()->SetNextScene("stage0"+std::to_string(stageNum_));
		}
	}

	oldLAnalog_ = IFE::Input::GetLXAnalog(range);
}

void Title::ImGUI()
{
	IFE::ImguiManager::Instance()->TextIntGUI(stageNum_);
	IFE::ImguiManager::Instance()->TextFloat3GUI(camera_->transform_->eye_);
	IFE::ImguiManager::Instance()->TextFloat3GUI(camera_->transform_->target_);
}
