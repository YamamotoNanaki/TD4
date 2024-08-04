#include "ClearAnimation.h"
#include"Scene.h"

void ClearAnimation::Initialize()
{
	player_= IFE::ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>();
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	actionCamera_= IFE::ObjectManager::Instance()->GetObjectPtr("PlayerActionCameraObject")->GetComponent<PlayerActionCamera>();
}

void ClearAnimation::Update()
{
	if (animationFlag_ == true)
	{
		AnimationInit();
		//PlayerMove();
		//CameraMove();
		//CameraRot();
		SceneChange();
		animationTimer_+= IFE::IFETime::sDeltaTime_;
	}
}

void ClearAnimation::Draw()
{
}

void ClearAnimation::Finalize()
{
}

void ClearAnimation::SetAnimationFlag(bool flag)
{
	animationFlag_ = flag;
}

bool ClearAnimation::GetAnimationFlag()
{
	return animationFlag_;
}

void ClearAnimation::AnimationInit()
{
	if (startFlag_ == false)
	{
		player_->SetClearFlag(true);
		action_->SetClearFlag(true);
		actionCamera_->SetClearFlag(true);
		action_->SetAnimation("standBy");

		startFlag_ = true;
	}
}

void ClearAnimation::PlayerMove()
{
	////’Ê˜H‘O‚ÌÀ•W‚ÉŒü‚©‚í‚¹‚éˆ—
	//if (animationTimer_ < maxAnimationTimer_ / 2)
	//{
	//	action_->transform_->position_.x = IFE::Lerp(action_->transform_->position_.x, 70.0f, maxAnimationTimer_ / 2, animationTimer_);
	//	action_->transform_->position_.y = IFE::Lerp(action_->transform_->position_.y, 22.0f, maxAnimationTimer_ / 2, animationTimer_);
	//	action_->transform_->position_.z = IFE::Lerp(action_->transform_->position_.z, -18.0f, maxAnimationTimer_ / 2, animationTimer_);
	//}
	//else
	//{
	//	//’Ê˜H‰œ‚ÌÀ•W‚ÉŒü‚©‚í‚¹‚éˆ—

	//}
}

void ClearAnimation::CameraMove()
{
	
}

void ClearAnimation::CameraRot()
{
}

void ClearAnimation::SceneChange()
{
	if (animationTimer_ > maxAnimationTimer_)
	{
		if (IFE::Scene::Instance()->GetSceneName() == "stage02")
		{
			IFE::Scene::Instance()->SetNextScene("stage03");
		}
		else if (IFE::Scene::Instance()->GetSceneName() == "stage03")
		{
			IFE::Scene::Instance()->SetNextScene("stage04");
		}
		else
		{
			IFE::Scene::Instance()->SetNextScene("GAMECLEAR");
		}
	}
}