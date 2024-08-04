#include "DeathAnimation.h"
#include"ColorBuffer.h"
#include "Input.h"
#include"Scene.h"

void DeathAnimation::Initialize()
{
	spritePtr_->order_ = 100;

	IFE::SpriteManager::Instance()->GetSpritePtr("restart")->order_ = 101;
	IFE::SpriteManager::Instance()->GetSpritePtr("toTitle")->order_ = 101;
	IFE::SpriteManager::Instance()->GetSpritePtr("youDied")->order_ = 101;
	IFE::SpriteManager::Instance()->GetSpritePtr("restart")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
	IFE::SpriteManager::Instance()->GetSpritePtr("toTitle")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
	IFE::SpriteManager::Instance()->GetSpritePtr("youDied")->GetComponent<IFE::ColorBuffer>()->SetAlpha(0.0f);
}

void DeathAnimation::Update()
{
	if (deathAnimationEndFlag_ == false)
	{
		Fade();
	}
	else
	{
		Select();
	}
}

void DeathAnimation::Draw()
{
}

void DeathAnimation::Finalize()
{
}

void DeathAnimation::SetDeathAnimationFlag(bool flag)
{
	deathAnimationFlag_ = flag;
}

void DeathAnimation::Fade()
{
	if (deathAnimationFlag_ == true)
	{
		if (deathFadeAnimationTime_ < maxDeathFadeAnimationTime_)
		{
			IFE::SpriteManager::Instance()->GetSpritePtr("gameOverFade")->GetComponent<IFE::ColorBuffer>()->SetAlpha(IFE::EaseInBack(0.0f, 0.75f, maxDeathFadeAnimationTime_, deathFadeAnimationTime_));
			IFE::SpriteManager::Instance()->GetSpritePtr("restart")->GetComponent<IFE::ColorBuffer>()->SetAlpha(IFE::EaseInBack(0.0f, 1.0f, maxDeathFadeAnimationTime_, deathFadeAnimationTime_));
			IFE::SpriteManager::Instance()->GetSpritePtr("toTitle")->GetComponent<IFE::ColorBuffer>()->SetAlpha(IFE::EaseInBack(0.0f, 0.5f, maxDeathFadeAnimationTime_, deathFadeAnimationTime_));
			IFE::SpriteManager::Instance()->GetSpritePtr("youDied")->GetComponent<IFE::ColorBuffer>()->SetAlpha(IFE::EaseInBack(0.0f, 0.5f, maxDeathFadeAnimationTime_, deathFadeAnimationTime_));
		}
		else
		{
			deathAnimationEndFlag_ = true;
		}

		deathFadeAnimationTime_ += IFE::IFETime::sDeltaTime_;
	}
}

void DeathAnimation::Select()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::LEFT) || oldLAnalog_ > -0.5f && IFE::Input::GetLXAnalog(controllerRange_) < -0.5f || IFE::Input::PadTrigger(IFE::PADCODE::LEFT))
	{
		IFE::SpriteManager::Instance()->GetSpritePtr("restart")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
		IFE::SpriteManager::Instance()->GetSpritePtr("toTitle")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f ,1.0f });
		isContinue_ = true;
	}
	else if (IFE::Input::GetKeyTrigger(IFE::Key::RIGHT) || oldLAnalog_ < 0.5f && IFE::Input::GetLXAnalog(controllerRange_) >0.5f || IFE::Input::PadTrigger(IFE::PADCODE::RIGHT))
	{
		IFE::SpriteManager::Instance()->GetSpritePtr("restart")->GetComponent<IFE::ColorBuffer>()->SetColor({ 0.25f,0.25f ,0.25f ,1.0f });
		IFE::SpriteManager::Instance()->GetSpritePtr("toTitle")->GetComponent<IFE::ColorBuffer>()->SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
		isContinue_ = false;
	}

	if (IFE::Input::PadTrigger(IFE::PADCODE::A) || IFE::Input::GetKeyTrigger(IFE::Key::Space))
	{
		if (isContinue_ == false)
		{
			IFE::Scene::Instance()->SetNextScene("TITLE");
		}
		else
		{
			IFE::Scene::Instance()->SetNextScene(IFE::Scene::Instance()->GetSceneName());
		}
	}

	oldLAnalog_ = IFE::Input::GetLXAnalog(controllerRange_);
}