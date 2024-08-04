#include "ClearAnimation.h"
#include"Scene.h"

void ClearAnimation::Initialize()
{
	player_= IFE::ObjectManager::Instance()->GetObjectPtr("Player")->GetComponent<Player>();
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	actionCamera_= IFE::ObjectManager::Instance()->GetObjectPtr("PlayerActionCamera")->GetComponent<PlayerActionCamera>();
}

void ClearAnimation::Update()
{
	if (animationFlag_ == true)
	{
		PlayerMove();
		CameraMove();
		CameraRot();
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
	animationTimer_ = flag;
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
		action_->SetAnimation("walk");

		if (IFE::Scene::Instance()->GetSceneName() == "stage02")
		{
			transform_->position_ = { 70.0f ,22.0f ,-18.0f };
			action_->transform_->rotation_.y = IFE::ConvertToDegrees(180.0f);
		}
		else if (IFE::Scene::Instance()->GetSceneName() == "stage03")
		{
			transform_->position_;
		}
		startFlag_ = true;
	}
}

void ClearAnimation::PlayerMove()
{
	//�ʘH�O�̍��W�Ɍ����킹�鏈��
	if (animationTimer_ < maxAnimationTimer_ / 2)
	{
		action_->transform_->position_.x = IFE::Lerp(action_->transform_->position_.x, 70.0f, maxAnimationTimer_ / 2, animationTimer_);
		action_->transform_->position_.y = IFE::Lerp(action_->transform_->position_.y, 22.0f, maxAnimationTimer_ / 2, animationTimer_);
		action_->transform_->position_.z = IFE::Lerp(action_->transform_->position_.z, -18.0f, maxAnimationTimer_ / 2, animationTimer_);
	}
	else
	{
		//�ʘH���̍��W�Ɍ����킹�鏈��

	}
}

void ClearAnimation::CameraMove()
{
	AnimationInit();

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
			IFE::Scene::Instance()->SetNextScene("Clear");
		}
	}
}