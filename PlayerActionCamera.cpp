#include "PlayerActionCamera.h"
#include"Transform.h"
#include "Collider.h"
#include "Input.h"
#include "IFETime.h"
#include"Object3D.h"

void PlayerActionCamera::Initialize()
{
	actionCamera_ = IFE::CameraManager::Instance()->GetCamera("ActionCamera");
	cameraAngle_.y = 180.0f;

	objectPtr_->DrawFlag_ = false;
}

void PlayerActionCamera::Update()
{
	if (ColliderHitFlag_)
	{
		float distance = 15.0f;
		float adjustedTimeValue = 15.0f;
		IFE::Complement(distance_, distance, adjustedTimeValue);
	}
}

void PlayerActionCamera::Draw()
{
}

void PlayerActionCamera::Finalize()
{
}

void PlayerActionCamera::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	if (hitCollider->attribute_ == static_cast<uint16_t>(IFE::Attribute::LANDSHAPE))
	{
		//���������Ƃ��̋����̑��
		IFE::Vector3 vec = myCollider->interPoint_ - IFE::Vector3(transform_->position_);
		if (distance_ > vec.Length())
		{
			distance_ = vec.Length();
			ColliderHitFlag_ = true;
		}
	}
}

IFE::Float3 PlayerActionCamera::GetPos()
{
	return transform_->position_;
}

IFE::Camera* PlayerActionCamera::GetCamera()
{
	return actionCamera_;
}

void PlayerActionCamera::CameraMove(const IFE::Vector3& playerPos)
{
	//��Ԏ��Ԓ����l
	const float adjustedTimeValue = 15.0f;
	//�J������Y���W���ߒl
	const float cameraYAdd = 7.5f;

	//���_�̈ړ���̍��W
	IFE::Vector3 eyeDestinationPos =
	{
		playerPos.x + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		playerPos.y + cameraYAdd + distance_ * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		playerPos.z + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//���_�̕�Ԉړ�
	IFE::Complement(transform_->position_.x, eyeDestinationPos.x, adjustedTimeValue);
	IFE::Complement(transform_->position_.y, eyeDestinationPos.y, adjustedTimeValue);
	IFE::Complement(transform_->position_.z, eyeDestinationPos.z, adjustedTimeValue);

	actionCamera_->transform_->eye_ = transform_->position_;

	//�����_�̈ړ���̍��W
	IFE::Vector3 targetDestinationPos =
	{
		playerPos.x - distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		playerPos.y - distance_ * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		playerPos.z - distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//�����_�̕�Ԉړ�
	IFE::Complement(actionCamera_->transform_->target_.x, targetDestinationPos.x, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->target_.y, targetDestinationPos.y, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->target_.z, targetDestinationPos.z, adjustedTimeValue);
}

void PlayerActionCamera::CameraRot()
{
	//��]���x
	const float rotSpeed = 80.0f;

#pragma region �L�[�{�[�h
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		cameraAngle_.y -= rotSpeed * IFE::IFETime::sDeltaTime_;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
	{
		cameraAngle_.y += rotSpeed * IFE::IFETime::sDeltaTime_;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::UP))
	{
		cameraAngle_.x -= rotSpeed * IFE::IFETime::sDeltaTime_;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
	{
		cameraAngle_.x += rotSpeed * IFE::IFETime::sDeltaTime_;
	}
#pragma endregion �L�[�{�[�h

#pragma region �R���g���[���[

	cameraAngle_ += {-IFE::Input::GetRYAnalog(controllerRange_) * rotSpeed * IFE::IFETime::sDeltaTime_, IFE::Input::GetRXAnalog(controllerRange_)* rotSpeed* IFE::IFETime::sDeltaTime_};

#pragma endregion �R���g���[���[

	//�c��]���E����
	cameraAngle_.x = std::clamp(cameraAngle_.x, -60.0f, 60.0f);
}

void PlayerActionCamera::CameraUpdate(const IFE::Vector3& playerPos)
{
	//�J�����̉�]
	CameraRot();

	//�J�������
	CameraMove(playerPos);
}