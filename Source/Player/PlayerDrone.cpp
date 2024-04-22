#include "PlayerDrone.h"
#include "Collider.h"
#include"Object3D.h"
#include "Input.h"
#include"Transform.h"

void PlayerDrone::SpeedZero(float& speed)
{
	if (speed > 0)
	{
		speed -= 0.03f;
		speed = max(speed, 0);
	}
	else
	{
		speed += 0.03f;
		speed = min(speed, 0);
	}
}

void PlayerDrone::Initialize()
{
	objectPtr_->DrawFlag_ = false;
}

void PlayerDrone::Update()
{
}

void PlayerDrone::Draw()
{
}

void PlayerDrone::Finalize()
{
}

void PlayerDrone::OnColliderHit(IFE::Collider collider)
{
}

void PlayerDrone::MoveUpdate()
{
	Move();
	Rotation();
}

void PlayerDrone::Move()
{
	/*float maxXSpeed = IFE::Input::GetLAnalog().x;
	float maxXSpeed = IFE::Input::GetLAnalog().x;*/

	if (IFE::Input::GetKeyPush(IFE::Key::A))
	{
		speed_.x -= 0.1f;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::D))
	{
		speed_.x += 0.1f;
	}
	
	if (IFE::Input::GetKeyPush(IFE::Key::W))
	{
		speed_.z += 0.1f;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::S))
	{
		speed_.z -= 0.1f;
	}
	
	if (IFE::Input::GetKeyPush(IFE::Key::Q))
	{
		speed_.y -= 0.1f;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::E))
	{
		speed_.y += 0.1f;
	}

	SpeedZero(speed_.x);
	SpeedZero(speed_.z);
	SpeedZero(speed_.y);

	transform_->position_ += speed_;
	//ƒXƒs[ƒhŒÀŠEˆ—
	const float maxSpeed = 0.5f;
	speed_.x = std::clamp(speed_.x, -maxSpeed, maxSpeed);
	speed_.y = std::clamp(speed_.y, -maxSpeed, maxSpeed);
	speed_.z = std::clamp(speed_.z, -maxSpeed, maxSpeed);
}

void PlayerDrone::Rotation()
{
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		transform_->eulerAngleDegrees_ += { 0, -5.0f, 0 };
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
	{
		transform_->eulerAngleDegrees_ += { 0, 5.0f, 0 };
	}
	
	if (IFE::Input::GetKeyPush(IFE::Key::UP))
	{
		transform_->eulerAngleDegrees_ += { -5.0f, 0, 0 };
	}if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
	{
		transform_->eulerAngleDegrees_ += { 5.0f, 0, 0 };
	}
	//c‰ñ“]‚ÌŒÀŠEˆ—
	const float maxVerticalRotation = 45.0f;
	transform_->eulerAngleDegrees_.x = std::clamp(transform_->eulerAngleDegrees_.x, -maxVerticalRotation, maxVerticalRotation);
}

bool PlayerDrone::GetDrawFlag()
{
	return objectPtr_->DrawFlag_;
}

void PlayerDrone::SetDrawFlag(bool flag)
{
	objectPtr_->DrawFlag_ = flag;
}

void PlayerDrone::SetPos(const IFE::Vector3& pos)
{
	transform_->position_ = pos;
}