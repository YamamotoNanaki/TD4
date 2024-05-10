#include "PlayerAction.h"
#include "Collider.h"
#include "Input.h"
#include"Transform.h"
#include"Object3D.h"

void PlayerAction::Initialize()
{
	actionCamera_ = IFE::CameraManager::Instance()->GetCamera("ActionCamera");
	cameraAngle_.y = 180.0f;
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
	CameraUpdate();
}

void PlayerAction::Move()
{
	if (IFE::Input::GetKeyPush(IFE::Key::A))
	{
		transform_->position_ += { -1, 0, 0 };
	}
	if (IFE::Input::GetKeyPush(IFE::Key::D))
	{
		transform_->position_ += { 1, 0, 0 };
	}if (IFE::Input::GetKeyPush(IFE::Key::W))
	{
		transform_->position_ += { 0, 0, 1 };
	}if (IFE::Input::GetKeyPush(IFE::Key::S))
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
	//ターゲットからの距離
	const float distance = 20.0f;

#pragma region カメラ移動
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		cameraAngle_.y -= 5;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
	{
		cameraAngle_.y += 5;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::UP))
	{
		cameraAngle_.x -= 5;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
	{
		cameraAngle_.x += 5;
	}
	cameraAngle_.x = std::clamp(cameraAngle_.x, -75.0f, 75.0f);
#pragma endregion カメラ移動

	actionCamera_->transform_->eye_ =
	{
		transform_->position_.x + distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		transform_->position_.y + distance * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		transform_->position_.z + distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
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