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
	droneCamera_ = IFE::CameraManager::Instance()->GetCamera("DroneCamera");
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

void PlayerDrone::OnColliderHit(IFE::ColliderCore collider)
{
}

void PlayerDrone::MoveUpdate()
{
	if (objectPtr_->DrawFlag_ == true)
	{
		objectPtr_->DrawFlag_ = false;
	}
	CameraUpdate();
	Rotation();
	Move();
}

void PlayerDrone::Move()
{
	//���x�N�g��
	IFE::Vector3 temporaryVec = { 0,1,0 };
	//�E�x�N�g���̍쐬
	IFE::Vector3 rightVec = frontVec_.Cross(temporaryVec);
	rightVec.Normalize();

	//�����Y���̓����͖����ėǂ�
	frontVec_.y = 0.0f;
	rightVec.y = 0.0f;

#pragma region �L�[�{�[�h
	if (IFE::Input::GetKeyPush(IFE::Key::A))
	{
		speed_ += rightVec;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::D))
	{
		speed_ -= rightVec;
	}if (IFE::Input::GetKeyPush(IFE::Key::W))
	{
		speed_ += frontVec_;
	}if (IFE::Input::GetKeyPush(IFE::Key::S))
	{
		speed_ -= frontVec_;
	}
	transform_->position_ += speed_;
#pragma endregion �L�[�{�[�h

#pragma region �R���g���[���[
	speed_ -= IFE::Input::GetLXAnalog(controllerRange_) * rightVec;
	speed_ += IFE::Input::GetLYAnalog(controllerRange_) * frontVec_;
	transform_->position_ += speed_;
#pragma endregion �R���g���[���[

	SpeedZero(speed_.x);
	SpeedZero(speed_.z);
	SpeedZero(speed_.y);

	//�X�s�[�h���E����
	const float maxSpeed = 0.5f;
	speed_.x = std::clamp(speed_.x, -maxSpeed, maxSpeed);
	speed_.y = std::clamp(speed_.y, -maxSpeed, maxSpeed);
	speed_.z = std::clamp(speed_.z, -maxSpeed, maxSpeed);

	transform_->UpdateMatrix();
}

void PlayerDrone::Rotation()
{
	const float cameraRotSpeed = 1.0;
#pragma region �L�[�{�[�h
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		transform_->eulerAngleDegrees_ += { 0, -cameraRotSpeed, 0 };
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
	{
		transform_->eulerAngleDegrees_ += { 0, cameraRotSpeed, 0 };
	}

	if (IFE::Input::GetKeyPush(IFE::Key::UP))
	{
		transform_->eulerAngleDegrees_ += { -cameraRotSpeed, 0, 0 };
	}if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
	{
		transform_->eulerAngleDegrees_ += { cameraRotSpeed, 0, 0 };
	}
#pragma endregion �L�[�{�[�h

#pragma region �R���g���[���[
	transform_->eulerAngleDegrees_ +=
	{
		-IFE::Input::GetRYAnalog(controllerRange_)* cameraRotSpeed,
		IFE::Input::GetRXAnalog(controllerRange_)* cameraRotSpeed,
		0
	};
#pragma endregion �R���g���[���[


	
	//�c��]�̌��E����
	const float maxVerticalRotation = 45.0f;
	transform_->eulerAngleDegrees_.x = std::clamp(transform_->eulerAngleDegrees_.x, -maxVerticalRotation, maxVerticalRotation);

	frontVec_ =
	{
		droneCamera_->transform_->target_.x - transform_->position_.x,
		droneCamera_->transform_->target_.y - transform_->position_.y,
		droneCamera_->transform_->target_.z - transform_->position_.z
	};
	frontVec_.Normalize();
}

void PlayerDrone::CameraUpdate()
{
	droneCamera_->transform_->eye_ = transform_->position_;

	const float targetDifference = 3.0f;
	droneCamera_->transform_->target_ =
	{
		transform_->position_.x + targetDifference * transform_->GetForwardVector().x,
		transform_->position_.y + targetDifference * transform_->GetForwardVector().y,
		transform_->position_.z + targetDifference * transform_->GetForwardVector().z,
	};
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