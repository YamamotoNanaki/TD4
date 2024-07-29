#include "DeathAnimation.h"
#include"ColorBuffer.h"

void DeathAnimation::Initialize()
{
}

void DeathAnimation::Update()
{
	Fade();
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
			IFE::SpriteManager::Instance()->GetSpritePtr("fade")->GetComponent<IFE::ColorBuffer>()->SetAlpha(IFE::EaseInBack(0.0f, 0.75f, maxDeathFadeAnimationTime_, deathFadeAnimationTime_));
		}

		deathFadeAnimationTime_ += IFE::IFETime::sDeltaTime_;
	}
}
