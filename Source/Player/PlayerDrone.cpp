#include "PlayerDrone.h"
#include "Collider.h"
#include"Object3D.h"
#include "Input.h"
#include"Transform.h"
#include"CameraManager.h"

void PlayerDrone::SpeedZero(float& speed)
{
	if (speed > 0)
	{
		speed -= 0.04f;
		speed = max(speed, 0);
	}
	else
	{
		speed += 0.04f;
		speed = min(speed, 0);
	}
}

void PlayerDrone::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	droneCamera_ = IFE::CameraManager::Instance()->GetCamera("ActionCamera");
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

void PlayerDrone::OnColliderHit(IFE::ColliderComponent collider)
{
}

void PlayerDrone::MoveUpdate()
{
	Rotation();
	Move();
	CameraUpdate();
}

void PlayerDrone::Move()
{
	/*float maxXSpeed = IFE::Input::GetLAnalog().x;
	float maxXSpeed = IFE::Input::GetLAnalog().x;*/

	float addSpeed = 0.05f;

	if (IFE::Input::GetKeyPush(IFE::Key::A))
	{
		speed_.x -= addSpeed;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::D))
	{
		speed_.x += addSpeed;
	}
	
	if (IFE::Input::GetKeyPush(IFE::Key::W))
	{
		speed_.z += addSpeed;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::S))
	{
		speed_.z -= addSpeed;
	}
	
	if (IFE::Input::GetKeyPush(IFE::Key::Q))
	{
		speed_.y -= addSpeed;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::E))
	{
		speed_.y += addSpeed;
	}

	SpeedZero(speed_.x);
	SpeedZero(speed_.z);
	SpeedZero(speed_.y);

	transform_->position_ += speed_;
	//スピード限界処理
	const float maxSpeed = 0.5f;
	speed_.x = std::clamp(speed_.x, -maxSpeed, maxSpeed);
	speed_.y = std::clamp(speed_.y, -maxSpeed, maxSpeed);
	speed_.z = std::clamp(speed_.z, -maxSpeed, maxSpeed);
}

void PlayerDrone::Rotation()
{
	//移動方向を変えずに向きだけ変わるカメラ(水平モード)
	if (cameraMode_ == false)
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
	}
	//カメラの向きに進むカメラ
	else
	{
		/*if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
		{
			transform_->eulerAngleDegrees_ += { 0, -5.0f, 0 };
		}
		if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
		{
			transform_->eulerAngleDegrees_ += { 0, 5.0f, 0 };
		}*/

		if (IFE::Input::GetKeyPush(IFE::Key::UP))
		{
			transform_->eulerAngleDegrees_ += transform_->GetForwardVector();
		}if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
		{
			transform_->eulerAngleDegrees_ -= transform_->GetForwardVector();
		}
	}
	
	//縦回転の限界処理
	const float maxVerticalRotation = 45.0f;
	transform_->eulerAngleDegrees_.x = std::clamp(transform_->eulerAngleDegrees_.x, -maxVerticalRotation, maxVerticalRotation);

	frontVec_ = front_ - pos_;
	frontVec_.Normalize();
}

void PlayerDrone::CameraUpdate()
{
	droneCamera_->transform_->eye_ = transform_->position_;

	//移動方向を変えずに向きだけ変わるカメラ(水平モード)
	if (cameraMode_ == false)
	{
		const float targetDifference = 3.0f;
		droneCamera_->transform_->target_ =
		{
			transform_->position_.x + targetDifference * transform_->GetForwardVector().x,
			transform_->position_.y + targetDifference * transform_->GetForwardVector().y,
			transform_->position_.z + targetDifference * transform_->GetForwardVector().z,
		};
	}
	//カメラの向きに進むカメラ
	else
	{

	}
	
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