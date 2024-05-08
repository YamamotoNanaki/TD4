#include "PlayerAction.h"
#include "Collider.h"
#include "Input.h"
#include"Transform.h"

void PlayerAction::Initialize()
{

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

void PlayerAction::Attack()
{
	if (IFE::Input::GetKeyPush(IFE::Key::Space))
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