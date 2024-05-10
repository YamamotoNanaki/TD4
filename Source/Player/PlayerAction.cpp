#include "PlayerAction.h"
#include "Collider.h"
#include "Input.h"
#include"Transform.h"
#include"Object3D.h"

void PlayerAction::Initialize()
{
	actionCamera_=IFE::CameraManager::Instance()->GetCamera("ActionCamera");
	IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
}

void PlayerAction::Update()
{
}

void PlayerAction::Draw()
{
}

void PlayerAction::Finalize()
{
}

void PlayerAction::OnColliderHit(IFE::Collider collider)
{
}

void PlayerAction::MoveUpdate()
{
	if (attackFlag_ == false)
	{
		Rotation();
		Move();
		objectPtr_->SetColor({ 1,1,1,1 });
	}
	else
	{
		objectPtr_->SetColor({ 1,0,0,1 });
	}
	Attack();
}

void PlayerAction::Move()
{
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		transform_->position_ += { -1,0,0 };
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
	{
		transform_->position_ += { 1, 0, 0 };
	}if (IFE::Input::GetKeyPush(IFE::Key::UP))
	{
		transform_->position_ += { 0, 0, 1 };
	}if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
	{
		transform_->position_ += { 0, 0, -1 };
	}
}

const IFE::Vector3 PlayerAction::GetPos()
{
	return transform_->position_;
}

void PlayerAction::Rotation()
{
	frontVec_ = front_ - pos_;
	frontVec_.Normalize();
}

void PlayerAction::CameraUpdate()
{
	const float distance = 1.0f;

	actionCamera_->transform_->eye_ =
	{
		distance * cosf(cameraAngle_.x) * sinf(cameraAngle_.y),
		distance * sinf(cameraAngle_.x),
		distance * cosf(cameraAngle_.x) * cosf(cameraAngle_.y)
	};
	actionCamera_->transform_->target_ = transform_->position_;
}

void PlayerAction::Attack()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space))
	{
		attackFlag_ = true;
	}

	if (attackFlag_ == true)
	{
		if (attackTimer_ > attackTime_)
		{
			attackFlag_ = false;
			attackTimer_ = 0;
		}
		attackTimer_++;
	}
}