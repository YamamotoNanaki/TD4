#include "PlayerAction.h"
#include "Collider.h"
#include "Input.h"
#include"Transform.h"
#include"Object3D.h"
#include"IFETime.h"
#include "EnemyAttack.h"

void PlayerAction::Initialize()
{
	actionCamera_ = IFE::CameraManager::Instance()->GetCamera("ActionCamera");
	transform_->position_.y = 2.0f;
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

void PlayerAction::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	//�G�U����e��
	if (myCollider->GetColliderType() == IFE::ColliderType::SPHERE)
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::EnemyAttack>()) {
			//�����������̏���
			/*hp--;*/
		}
	}
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
	const float speed = 10.0f;

	//���ʃx�N�g���̍쐬
	frontVec_ = transform_->position_ - actionCamera_->transform_->eye_;
	frontVec_.Normalize();
	//���x�N�g��
	temporaryVec_ = { 0,1,0 };
	//�E�x�N�g���̍쐬
	rightVec_ = frontVec_.Cross(temporaryVec_);
	rightVec_.Normalize();

	//�����Y���̓����͖����ėǂ�
	frontVec_.y = 0.0f;
	rightVec_.y = 0.0f;

#pragma region �L�[�{�[�h
	if (IFE::Input::GetKeyPush(IFE::Key::A))
	{
		transform_->position_ += rightVec_ * speed * IFE::IFETime::sDeltaTime_;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::D))
	{
		transform_->position_ -= rightVec_ * speed * IFE::IFETime::sDeltaTime_;
	}if (IFE::Input::GetKeyPush(IFE::Key::W))
	{
		transform_->position_ += frontVec_ * speed * IFE::IFETime::sDeltaTime_;
	}if (IFE::Input::GetKeyPush(IFE::Key::S))
	{
		transform_->position_ -= frontVec_ * speed * IFE::IFETime::sDeltaTime_;
	}
#pragma endregion �L�[�{�[�h

#pragma region �R���g���[���[
	transform_->position_ -= IFE::Input::GetLXAnalog(controllerRange_) * rightVec_ * speed * IFE::IFETime::sDeltaTime_;
	transform_->position_ += IFE::Input::GetLYAnalog(controllerRange_) * frontVec_ * speed * IFE::IFETime::sDeltaTime_;
#pragma endregion
}

const IFE::Vector3 PlayerAction::GetPos()
{
	return transform_->position_;
}

IFE::Camera* PlayerAction::GetActionCamera()
{
	return actionCamera_;
}

void PlayerAction::Rotation()
{
	float lx = IFE::Input::GetLXAnalog(controllerRange_);
	float ly = IFE::Input::GetLYAnalog(controllerRange_);

	float angleY = 0;
	if (lx <= 0 || ly != 0)
	{
		//�����x�N�g���̊p�x+�R���g���[���[�̊p�x
		angleY = IFE::ConvertToDegrees(std::atan2(frontVec_.x, frontVec_.z) + std::atan2(lx, ly));
		transform_->eulerAngleDegrees_.y = angleY;
	}
}

void PlayerAction::CameraUpdate()
{
	//�J�����̉�]
	CameraRot();

	//�J�������
	CameraComplement();
}

void PlayerAction::Attack()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::X))
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

void PlayerAction::CameraComplement()
{
	//�^�[�Q�b�g����̋���
	const float distance = 15.0f;
	//��Ԏ��Ԓ����l
	const float adjustedTimeValue = 15.0f;
	//�J������Y���W���ߒl
	const float cameraYAdd = 7.5f;

	//���_�̈ړ���̍��W
	IFE::Vector3 eyeDestinationPos =
	{
		transform_->position_.x + distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		transform_->position_.y + cameraYAdd + distance * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		transform_->position_.z + distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//���_�̕�Ԉړ�
	IFE::Complement(actionCamera_->transform_->eye_.x, eyeDestinationPos.x, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->eye_.y, eyeDestinationPos.y, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->eye_.z, eyeDestinationPos.z, adjustedTimeValue);

	//�����_�̈ړ���̍��W
	IFE::Vector3 targetDestinationPos =
	{
		transform_->position_.x - distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		transform_->position_.y - distance * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		transform_->position_.z - distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//�����_�̕�Ԉړ�
	IFE::Complement(actionCamera_->transform_->target_.x, targetDestinationPos.x, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->target_.y, targetDestinationPos.y, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->target_.z, targetDestinationPos.z, adjustedTimeValue);
}

void PlayerAction::CameraRot()
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