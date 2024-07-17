#include "PlayerDrone.h"
#include "Collider.h"
#include"Object3D.h"
#include "Input.h"
#include"Transform.h"
#include"CameraManager.h"
#include"IFETime.h"
#include "Sound.h"

void PlayerDrone::SpeedZero(float& speed, float decelerationSpeed)
{
	if (speed > 0)
	{
		speed -= decelerationSpeed;
		speed = max(speed, 0);
	}
	else
	{
		speed += decelerationSpeed;
		speed = min(speed, 0);
	}
}

void PlayerDrone::Initialize()
{
	objectPtr_->DrawFlag_ = false;
	droneCamera_ = IFE::CameraManager::Instance()->GetCamera("DroneCamera");
	transform_->rotation_.x = 0.0f;
	transform_->scale_ = { 0.7f,0.7f,0.7f };

	//プロペラ
	for (uint8_t i = 0; i < 4; i++)
	{
		auto ptr = IFE::ObjectManager::Instance()->AddInitialize("propeller", IFE::ModelManager::Instance()->GetModel("propeller"));
		ptr->AddComponent < IFE::Propeller >();
		propeller_[i] = ptr->GetComponent<IFE::Propeller>();
		propeller_[i]->transform_->parent_ = transform_;
	}
	propeller_[0]->objectPtr_->transform_->position_ += {0.8f, 0.1f, 0.8f};
	propeller_[1]->objectPtr_->transform_->position_ += {-0.8f, 0.1f, 0.8f};
	propeller_[2]->objectPtr_->transform_->position_ += {0.8f, 0.1f, -0.8f};
	propeller_[3]->objectPtr_->transform_->position_ += {-0.8f, 0.1f, -0.8f};
}

void PlayerDrone::Update()
{
	const float maxTime = 2.0f;
	time_+= IFE::IFETime::sDeltaTime_;
	if (time_ > maxTime)
	{
		time_ = 0;
	}

	if (objectPtr_->DrawFlag_ == true)
	{
		transform_->position_.y = dronePosY_ + IFE::SimpleHarmonicMotion(time_, 0.1f, maxTime);
		//プロペラ回転
		for (uint8_t i = 0; i < 4; i++)
		{
			propeller_[i]->objectPtr_->DrawFlag_ = true;
			propeller_[i]->Turn();
		}
	}
	else
	{
		//プロペラの描画フラグfalse
		for (uint8_t i = 0; i < 4; i++)
		{
			propeller_[i]->objectPtr_->DrawFlag_ = false;
		}
	}
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
	//仮ベクトル
	IFE::Vector3 temporaryVec = { 0,1,0 };
	//右ベクトルの作成
	IFE::Vector3 rightVec = frontVec_.Cross(temporaryVec);
	rightVec.Normalize();

	//今回はY軸の動きは無くて良い
	frontVec_.y = 0.0f;
	rightVec.y = 0.0f;

	const float speed = 1.0f;

#pragma region キーボード
	if (IFE::Input::GetKeyPush(IFE::Key::A))
	{
		moveValue_ += rightVec;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::D))
	{
		moveValue_ -= rightVec;
	}if (IFE::Input::GetKeyPush(IFE::Key::W))
	{
		moveValue_ += frontVec_;
	}if (IFE::Input::GetKeyPush(IFE::Key::S))
	{
		moveValue_ -= frontVec_;
	}if (IFE::Input::GetKeyPush(IFE::Key::Q))
	{
		moveValue_.y++;

	}
	if (IFE::Input::GetKeyPush(IFE::Key::E))
	{
		moveValue_.y--;
	}
#pragma endregion キーボード

#pragma region コントローラー
	moveValue_ -= IFE::Input::GetLXAnalog(controllerRange_) * rightVec;
	moveValue_ += IFE::Input::GetLYAnalog(controllerRange_) * frontVec_;

	if (IFE::Input::GetRTrigger())
	{
		moveValue_.y++;
	}
	if (IFE::Input::GetLTrigger())
	{
		moveValue_.y--;
	}

#pragma endregion コントローラー

#pragma region 統一処理

	if (IFE::Input::GetKeyTrigger(IFE::Key::LSHIFT) || IFE::Input::GetKeyTrigger(IFE::Key::RSHIFT) || IFE::Input::PadTrigger(IFE::PADCODE::LTHUMB))
	{
		slowSpeedMode_ = !slowSpeedMode_;
	}

	//減速速度
	float decelerationSpeed = 0.0f;

	//スピード限界処理
	float maxSpeed = 0.0f;

	if (slowSpeedMode_ == false)
	{
		decelerationSpeed = 0.5f;
		maxSpeed = 10.0f;
	}
	else
	{
		decelerationSpeed = 0.25f;
		maxSpeed = 5.0f;
	}

	SpeedZero(moveValue_.x, decelerationSpeed);
	SpeedZero(moveValue_.z, decelerationSpeed);
	SpeedZero(moveValue_.y, decelerationSpeed);

	moveValue_.x = std::clamp(moveValue_.x, -maxSpeed, maxSpeed);
	moveValue_.y = std::clamp(moveValue_.y, -maxSpeed, maxSpeed);
	moveValue_.z = std::clamp(moveValue_.z, -maxSpeed, maxSpeed);

	transform_->position_ += moveValue_ * speed * IFE::IFETime::sDeltaTime_;

	transform_->UpdateMatrix();
#pragma endregion

	//if (objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->onGround_ == true)
	//{
	//	transform_->position_.y += 0.2f;
	//}

	//MoveLimit();
}

void PlayerDrone::MoveLimit()
{
	if (transform_->position_.y > 18.0f)
	{
		transform_->position_.y = 18.0f;
	}
}

void PlayerDrone::Rotation()
{
	const float cameraRotSpeed = 100.0f * IFE::IFETime::sDeltaTime_;
#pragma region キーボード
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		transform_->rotation_ += { 0, -cameraRotSpeed, 0 };
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
	{
		transform_->rotation_ += { 0, cameraRotSpeed, 0 };
	}

	if (IFE::Input::GetKeyPush(IFE::Key::UP))
	{
		transform_->rotation_ += { -cameraRotSpeed, 0, 0 };
	}if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
	{
		transform_->rotation_ += { cameraRotSpeed, 0, 0 };
	}
#pragma endregion キーボード

#pragma region コントローラー
	transform_->rotation_ +=
	{
		-IFE::Input::GetRYAnalog(controllerRange_) * cameraRotSpeed,
			IFE::Input::GetRXAnalog(controllerRange_)* cameraRotSpeed,
			0
	};
#pragma endregion コントローラー

	//縦回転の限界処理
	const float maxVerticalRotation = 45.0f;
	transform_->rotation_.x = std::clamp(transform_->rotation_.x, -maxVerticalRotation, maxVerticalRotation);

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
	if (flag == true)
	{
		dronePosY_ = transform_->position_.y;
	}
}

void PlayerDrone::SetPos(const IFE::Vector3& pos)
{
	transform_->position_ = pos;
}

IFE::Vector3 PlayerDrone::GetPos()
{
	return transform_->position_;
}

void PlayerDrone::SetRotY(const float rot)
{
	transform_->rotation_.y = rot;
}

IFE::Camera* PlayerDrone::GetDroneCamera()
{
	return droneCamera_;
}

void PlayerDrone::SetIsDroneSurvival(bool flag)
{
	isDroneSurvival_ = flag;
}

bool PlayerDrone::GetIsDroneSurvival()
{
	return isDroneSurvival_;
}