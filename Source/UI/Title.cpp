#include "Title.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"
#include "Over.h"
#include"Transform.h"
#include"Ease.h"
#include"ColorBuffer.h"

using namespace IFE;

void Title::Initialize()
{
	camera_ = IFE::CameraManager::Instance()->GetCamera("titleCamera");
	camera_->transform_->eye_ = { 0.0f,5.0f,-cDistance_ };
	camera_->transform_->target_ = { 0.0f,20.0f,20.0f };
	cameraAngle_ = static_cast<float>(-PI) / 2;
	IFE::CameraManager::Instance()->SetActiveCamera("titleCamera");

	flamePos_[0] = { -28.0f,7.0f,-12.0f };
	flamePos_[1] = { 2.0f,7.0f,-12.0f };
	flamePos_[2] = { -13.0f,27.5f,15.0f };

	IFE::Sound::Instance()->LoadWave("title",IFE::Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("title", 50);
	isNext = false;

	IFE::SpriteManager::Instance()->GetSpritePtr("yes")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
	IFE::SpriteManager::Instance()->GetSpritePtr("no")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
}

void Title::Update()
{
	switch (titleSelectFlag_)
	{
	case TitleSelect::TITLE:
		TitleLogoSimpleHarmonicMotion();
		CameraRot();
		if (animationFlag_ == false)
		{
			if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
			{
				beforeEaseAngle_ = cameraAngle_;
				beforeGameLogoPosY_ = IFE::SpriteManager::Instance()->GetSpritePtr("gameLogo")->transform_->position2D_.y;
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
				titleAnimationTimer_ = 0.0f;
				animationFlag_ = true;
			}
			Select();
			SelectFlame();
		}
		else
		{
			BackTitleAnimation();
		}
		break;
	case TitleSelect::SELECT2:
		SelectCheck();
		break;
	default:
		break;
	}
}

void Title::TitleLogoSimpleHarmonicMotion()
{
	titleAnimationTimer_ += IFE::IFETime::sDeltaTime_;
	IFE::SpriteManager::Instance()->GetSpritePtr("gameLogo")->transform_->position2D_.y = 200.0f + IFE::SimpleHarmonicMotion(titleAnimationTimer_, 10.0f, 3.0f);
}

void Title::CameraRot()
{
	if (animationFlag_ == false && titleSelectFlag_ == TitleSelect::TITLE)
	{
		cameraAngle_ += 0.1f * IFE::IFETime::sDeltaTime_;

		if (cameraAngle_ >= 2 * PI)
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
	distanse_ = IFE::EaseInOutQuart(animationTime_, cDistance_, 0.1f, maxAnimationTime_);
	camera_->transform_->eye_.y = IFE::EaseInOutBack(animationTime_, 5.0f, cDistance_, maxAnimationTime_);
	if (beforeEaseAngle_ >= PI / 2)
	{
		cameraAngle_ = IFE::EaseInOutQuart(animationTime_, beforeEaseAngle_, static_cast<float>(PI) * 3 / 2, maxAnimationTime_);
	}
	else
	{
		cameraAngle_ = IFE::EaseInOutQuart(animationTime_, beforeEaseAngle_, static_cast<float>(-PI) / 2, maxAnimationTime_);
	}

	//UI
	IFE::SpriteManager::Instance()->GetSpritePtr("gameLogo")->transform_->position2D_.y = IFE::EaseInOutBack(animationTime_, 200.0f, -200.0f, maxAnimationTime_);
	IFE::SpriteManager::Instance()->GetSpritePtr("pressButton")->transform_->position2D_.y = IFE::EaseInOutBack(animationTime_, 880.0f, 1280.0f, maxAnimationTime_);

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
	distanse_ = IFE::EaseInOutQuart(animationTime_, 0.1f, cDistance_, maxAnimationTime_);
	camera_->transform_->eye_.y = IFE::EaseInOutQuart(animationTime_, cDistance_, 5.0f, maxAnimationTime_);

	//UI
	IFE::SpriteManager::Instance()->GetSpritePtr("gameLogo")->transform_->position2D_.y = IFE::EaseInOutBack(animationTime_, -200.0f, 200.0f, maxAnimationTime_);
	IFE::SpriteManager::Instance()->GetSpritePtr("pressButton")->transform_->position2D_.y = IFE::EaseInOutBack(animationTime_, 1280.0f, 880.0f, maxAnimationTime_);

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
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT) || oldLAnalog_.x > -0.5f && IFE::Input::GetLXAnalog(range) < -0.5f)
	{
		stageNum_ = 1;
		oldFlamePos = IFE::ObjectManager::Instance()->GetObjectPtr("frame")->transform_->position_;
		flameAnimationTime_ = 0.0f;
		flameAnimationFlag_ = true;
	}
	else if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_.x < 0.5f && IFE::Input::GetLXAnalog(range) >0.5f)
	{
		stageNum_ = 2;
		oldFlamePos = IFE::ObjectManager::Instance()->GetObjectPtr("frame")->transform_->position_;
		flameAnimationTime_ = 0.0f;
		flameAnimationFlag_ = true;
	}

	if (IFE::Input::GetKeyTrigger(IFE::Key::UP) || oldLAnalog_.y > -0.5f && IFE::Input::GetLYAnalog(range) < -0.5f)
	{
		stageNum_ = 3;
		oldFlamePos = IFE::ObjectManager::Instance()->GetObjectPtr("frame")->transform_->position_;
		flameAnimationTime_ = 0.0f;
		flameAnimationFlag_ = true;
	}
	else if(IFE::Input::GetKeyTrigger(IFE::Key::DOWN) || oldLAnalog_.y < 0.5f && IFE::Input::GetLYAnalog(range) >0.5f)
	{
		stageNum_ = 2;
		oldFlamePos = IFE::ObjectManager::Instance()->GetObjectPtr("frame")->transform_->position_;
		flameAnimationTime_ = 0.0f;
		flameAnimationFlag_ = true;
	}

	//ƒtƒ‰ƒO‚ªŒÀŠE‚ð’´‚¦‚È‚¢ˆ—
	if (stageNum_ > 3)
	{
		stageNum_ = 1;
	}
	if (stageNum_ < 1)
	{
		stageNum_ = 3;
	}

	oldLAnalog_ = IFE::Input::GetLXAnalog(range);

	if (flameAnimationFlag_ == false)
	{
		if (Input::PadTrigger(PADCODE::A) || Input::GetKeyTrigger(Key::Space))
		{
			oldLAnalog_ = 0.0f;
			IFE::SpriteManager::Instance()->GetSpritePtr("yes")->GetComponent<IFE::ColorBuffer>()->SetColor({1.0f,1.0f ,1.0f ,1.0f });
			IFE::SpriteManager::Instance()->GetSpritePtr("no")->GetComponent<IFE::ColorBuffer>()->SetColor({0.25f,0.25f ,0.25f ,1.0f });
			titleSelectFlag_ = TitleSelect::SELECT2;
		}
	}
}

void Title::SelectCheck()
{
	int32_t range = 10000;
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT) || oldLAnalog_.x > -0.5f && IFE::Input::GetLXAnalog(range) < -0.5f)
	{
		IFE::SpriteManager::Instance()->GetSpritePtr("yes")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
		IFE::SpriteManager::Instance()->GetSpritePtr("no")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f ,1.0f });
		checkFlag_ = true;
	}
	else if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_.x < 0.5f && IFE::Input::GetLXAnalog(range) >0.5f)
	{
		IFE::SpriteManager::Instance()->GetSpritePtr("yes")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f ,1.0f });
		IFE::SpriteManager::Instance()->GetSpritePtr("no")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
		checkFlag_ = false;
	}

	if (Input::PadTrigger(PADCODE::B) || Input::GetKeyTrigger(Key::Esc))
	{
		oldLAnalog_ = 0.0f;
		IFE::SpriteManager::Instance()->GetSpritePtr("yes")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
		IFE::SpriteManager::Instance()->GetSpritePtr("no")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
		titleSelectFlag_ = TitleSelect::SELECT;
		checkFlag_ = true;
	}

	if (Input::PadTrigger(PADCODE::A) || Input::GetKeyTrigger(Key::Space))
	{
		if (checkFlag_ == false)
		{
			oldLAnalog_ = 0.0f;
			IFE::SpriteManager::Instance()->GetSpritePtr("yes")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
			IFE::SpriteManager::Instance()->GetSpritePtr("no")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
			titleSelectFlag_ = TitleSelect::SELECT;
		}
		else
		{
			if (!isNext) {
				IFE::Sound::Instance()->SoundPlay("title", false, true);
			}
			isNext = true;

			Scene::Instance()->SetNextScene("stage0" + std::to_string(stageNum_ + 1));
		}
	}

	oldLAnalog_ = IFE::Input::GetLAnalog(range);
}

void Title::SelectFlame()
{
	if (flameAnimationFlag_ == true)
	{
		if (flameAnimationTime_ > maxFlameAnimationTime_)
		{
			flameAnimationFlag_ = false;
		}

		IFE::ObjectManager::Instance()->GetObjectPtr("frame")->transform_->position_;
		IFE::ObjectManager::Instance()->GetObjectPtr("frame")->transform_->position_ =
		{
			IFE::EaseInOutQuart(flameAnimationTime_, oldFlamePos.x, flamePos_[stageNum_ - 1].x, maxFlameAnimationTime_),
			IFE::EaseInOutQuart(flameAnimationTime_, oldFlamePos.y, flamePos_[stageNum_ - 1].y, maxFlameAnimationTime_),
			IFE::EaseInOutQuart(flameAnimationTime_, oldFlamePos.z, flamePos_[stageNum_ - 1].z, maxFlameAnimationTime_)
		};

		flameAnimationTime_ += IFE::IFETime::sDeltaTime_;
	}
}
