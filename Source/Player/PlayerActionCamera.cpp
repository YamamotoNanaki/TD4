#include "PlayerActionCamera.h"
#include"Transform.h"
#include "Collider.h"
#include "Input.h"
#include "IFETime.h"
#include"Object3D.h"
#include"Player.h"

void PlayerActionCamera::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	config_ = IFE::SpriteManager::Instance()->GetSpritePtr("config")->GetComponent<Config>();
}

void PlayerActionCamera::CameraInitialize(const IFE::Vector3& playerPos)
{
	actionCamera_ = IFE::CameraManager::Instance()->GetCamera("ActionCamera");
	cameraAngle_.y = 180.0f;

	transform_->position_ =
	{
		playerPos.x + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		playerPos.y + cameraYAdd_ + distance_ * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		playerPos.z + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	actionCamera_->transform_->eye_ = transform_->position_;
	player_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction");
}

void PlayerActionCamera::Update()
{
	if (clearFlag_ == true)return;

	IFE::Vector3 v = player_->transform_->position_ - transform_->position_;
	defaultRayDistance_ = v.Length();
	v.Normalize();
	objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->rayDir_ = v;
	if (!ColliderHitFlag_)
	{
		float distance = defaultDistance_;
		float adjustedTimeValue = defaultDistance_;
		IFE::Complement(distance_, distance, adjustedTimeValue);
	}
	ColliderHitFlag_ = false;
}

void PlayerActionCamera::Draw()
{
}

void PlayerActionCamera::Finalize()
{
}

void PlayerActionCamera::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	myCollider;
	if (hitCollider->attribute_ & static_cast<uint16_t>(IFE::Attribute::LANDSHAPE))
	{
		float dis = defaultRayDistance_ - myCollider->rayDistanceFar;
		if (dis <= 0)return;
		//���������Ƃ��̋����̑��
		if (distance_ >= dis)
		{
			distance_ = dis;
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

void PlayerActionCamera::SetClearFlag(bool flag)
{
	clearFlag_ = flag;
}

void PlayerActionCamera::CameraMove(const IFE::Vector3& playerPos)
{
	//���_�̈ړ���̍��W
	IFE::Vector3 eyeDestinationPos =
	{
		playerPos.x + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		playerPos.y + cameraYAdd_ + distance_ * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		playerPos.z + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//���_�̕�Ԉړ�
	IFE::Complement(transform_->position_.x, eyeDestinationPos.x, adjustedTimeValue_);
	IFE::Complement(transform_->position_.y, eyeDestinationPos.y, adjustedTimeValue_);
	IFE::Complement(transform_->position_.z, eyeDestinationPos.z, adjustedTimeValue_);

	actionCamera_->transform_->eye_ = transform_->position_;

	//�����_�̈ړ���̍��W
	IFE::Vector3 targetDestinationPos =
	{
		playerPos.x - distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		playerPos.y - distance_ * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		playerPos.z - distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//�����_�̕�Ԉړ�
	IFE::Complement(actionCamera_->transform_->target_.x, targetDestinationPos.x, adjustedTimeValue_);
	IFE::Complement(actionCamera_->transform_->target_.y, targetDestinationPos.y, adjustedTimeValue_);
	IFE::Complement(actionCamera_->transform_->target_.z, targetDestinationPos.z, adjustedTimeValue_);
}

void PlayerActionCamera::CameraRot()
{
	float rotSpeed = 0.0f;
	//��]���x
	if (config_->GetConfigValue().cameraReverse == false)
	{
		rotSpeed = 80.0f;
	}
	else
	{
		rotSpeed = -80.0f;
	}

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