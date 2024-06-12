#include "PlayerAction.h"
#include "Collider.h"
#include "Input.h"
#include"Transform.h"
#include"Object3D.h"
#include"IFETime.h"
#include "EnemyAttack.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include"Scene.h"

void PlayerAction::Initialize()
{
	transform_->position_.y = 2.0f;

	camera_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerActionCameraObject")->GetComponent<PlayerActionCamera>();
	camera_->CameraInitialize(transform_->position_);

	//HP
	auto hpPtr = IFE::SpriteManager::Instance()->GetSpritePtr("playerHp")->GetComponent<IFE::PlayerHp>();
	playerHp_ = hpPtr;
	playerHp_->SetHp(hp_);

	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("PlayerAttack", IFE::ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<PlayerAttack>();
	playerAttack_ = ptr->GetComponent<PlayerAttack>();
	playerAttack_->transform_->parent_ = transform_;
	playerAttack_->objectPtr_->transform_->position_ += {0, 0, 2};
	playerAttack_->SetName("player");
}

void PlayerAction::Update()
{
	//hitcool
	if (isHit_ == true)
	{
		if (hp_ > 0)
		{
			playerHp_->ScaleCalc(hp_, 1, hitTime_, HIT_COOLTIME);
		}

		hitTime_ -= IFE::IFETime::sDeltaTime_;
		if (hitTime_ <= 0) {
			isHit_ = false;
		}
	}

	if (hp_ <= 0)
	{
		IFE::Scene::Instance()->SetNextScene("GAMEOVER");
	}
}

void PlayerAction::Draw()
{
}

void PlayerAction::Finalize()
{
	delete playerAttack_;
}

void PlayerAction::DecHp()
{
	if (isHit_ == false) {
		hp_--;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
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
	camera_->CameraUpdate(transform_->position_);
}

void PlayerAction::Move()
{
	const float speed = 10.0f;

	//���ʃx�N�g���̍쐬
	frontVec_ = transform_->position_ - camera_->GetPos();
	frontVec_.Normalize();
	//���x�N�g��
	temporaryVec_ = { 0,1,0 };
	//�E�x�N�g���̍쐬
	rightVec_ = frontVec_.Cross(temporaryVec_);
	rightVec_.Normalize();

	//�����Y���̓����͖����ėǂ�
	frontVec_.y = 0.0f;
	rightVec_.y = 0.0f;
	frontVec_.Normalize();
	rightVec_.Normalize();

	if (objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->onGround_ == false)
	{
		transform_->position_.y = -4.9f * IFE::IFETime::sDeltaTime_;
	}

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
	return camera_->GetCamera();
}

const bool PlayerAction::GetAttackFlag()
{
	return attackFlag_;
}

const IFE::Vector3 PlayerAction::GetFrontVec()
{
	return frontVec_;
}

const float PlayerAction::GetRotY()
{
	return rotY_;
}

void PlayerAction::Rotation()
{
#pragma region �L�[�{�[�h
	float kx = 0.0f;
	float ky = 0.0f;

	if (IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::D) || IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::S))
	{
		if (IFE::Input::GetKeyPush(IFE::Key::A))
		{
			kx = -1.0f;
		}
		if (IFE::Input::GetKeyPush(IFE::Key::D))
		{
			kx = 1.0f;;
		}
		if (IFE::Input::GetKeyPush(IFE::Key::W))
		{
			ky = 1.0f;
		}
		if (IFE::Input::GetKeyPush(IFE::Key::S))
		{
			ky = -1.0f;
		}

		rotY_ = IFE::ConvertToDegrees(std::atan2(frontVec_.x, frontVec_.z) + std::atan2(kx, ky));
		transform_->eulerAngleDegrees_.y = rotY_;
	}
#pragma endregion �L�[�{�[�h

#pragma region �R���g���[���[
	float lx = IFE::Input::GetLXAnalog(controllerRange_);
	float ly = IFE::Input::GetLYAnalog(controllerRange_);

	if (lx != 0 || ly != 0)
	{
		//�����x�N�g���̊p�x+�R���g���[���[�̊p�x
		rotY_ = IFE::ConvertToDegrees(std::atan2(frontVec_.x, frontVec_.z) + std::atan2(lx, ly));
		transform_->eulerAngleDegrees_.y = rotY_;
	}
#pragma endregion �R���g���[���[
}

void PlayerAction::Attack()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::X))
	{
		attackFlag_ = true;
		playerAttack_->objectPtr_->DrawFlag_ = true;
	}

	if (attackFlag_ == true)
	{
		if (attackTimer_ > attackTime_)
		{
			attackFlag_ = false;
			attackTimer_ = 0;
			playerAttack_->objectPtr_->DrawFlag_ = false;
		}

		attackTimer_++;
	}

	playerAttack_->SetIsAttack(attackFlag_);
}