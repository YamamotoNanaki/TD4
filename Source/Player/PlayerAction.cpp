#include "PlayerAction.h"
#include "Collider.h"
#include "Input.h"
#include"Transform.h"
#include"Object3D.h"
#include"IFETime.h"
#include "EnemyAttack.h"
#include "ObjectManager.h"
#include "ModelManager.h"

void PlayerAction::Initialize()
{
	actionCamera_ = IFE::CameraManager::Instance()->GetCamera("ActionCamera");

	transform_->position_.y = 2.0f;
	cameraAngle_.y = 180.0f;

	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("PlayerAttack", IFE::ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<PlayerAttack>();
	playerAttack_ = ptr->GetComponent<PlayerAttack>();
	playerAttack_->transform_->parent_ = transform_;
	playerAttack_->objectPtr_->transform_->position_ += {0, 0, 2};
	playerAttack_->SetName("player");
}

void PlayerAction::Update()
{
	/*playerHp_->ScaleCalc(hp);*/
}

void PlayerAction::Draw()
{
}

void PlayerAction::Finalize()
{
	delete playerAttack_;
}

void PlayerAction::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	//敵攻撃被弾時
	if (myCollider->GetColliderType() == IFE::ColliderType::SPHERE)
	{
		if (hitCollider->objectPtr_->GetComponent<IFE::EnemyAttack>()) {
			//当たった時の処理
			hp--;
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

	//正面ベクトルの作成
	frontVec_ = transform_->position_ - actionCamera_->transform_->eye_;
	frontVec_.Normalize();
	//仮ベクトル
	temporaryVec_ = { 0,1,0 };
	//右ベクトルの作成
	rightVec_ = frontVec_.Cross(temporaryVec_);
	rightVec_.Normalize();

	//今回はY軸の動きは無くて良い
	frontVec_.y = 0.0f;
	rightVec_.y = 0.0f;

	if (objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->onGround_ == false)
	{
		transform_->position_.y = -4.9f * IFE::IFETime::sDeltaTime_;
	}

#pragma region キーボード
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
#pragma endregion キーボード

#pragma region コントローラー
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
#pragma region キーボード
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
#pragma endregion キーボード

#pragma region コントローラー
	float lx = IFE::Input::GetLXAnalog(controllerRange_);
	float ly = IFE::Input::GetLYAnalog(controllerRange_);

	if (lx != 0 || ly != 0)
	{
		//方向ベクトルの角度+コントローラーの角度
		rotY_ = IFE::ConvertToDegrees(std::atan2(frontVec_.x, frontVec_.z) + std::atan2(lx, ly));
		transform_->eulerAngleDegrees_.y = rotY_;
	}
#pragma endregion コントローラー
}

void PlayerAction::CameraUpdate()
{
	//カメラの回転
	CameraRot();

	//カメラ補間
	CameraComplement();
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

void PlayerAction::CameraComplement()
{
	//ターゲットからの距離
	const float distance = 15.0f;
	//補間時間調整値
	const float adjustedTimeValue = 15.0f;
	//カメラのY座標調節値
	const float cameraYAdd = 7.5f;

	//視点の移動先の座標
	IFE::Vector3 eyeDestinationPos =
	{
		transform_->position_.x + distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		transform_->position_.y + cameraYAdd + distance * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		transform_->position_.z + distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//視点の補間移動
	IFE::Complement(actionCamera_->transform_->eye_.x, eyeDestinationPos.x, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->eye_.y, eyeDestinationPos.y, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->eye_.z, eyeDestinationPos.z, adjustedTimeValue);

	//注視点の移動先の座標
	IFE::Vector3 targetDestinationPos =
	{
		transform_->position_.x - distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		transform_->position_.y - distance * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		transform_->position_.z - distance * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//注視点の補間移動
	IFE::Complement(actionCamera_->transform_->target_.x, targetDestinationPos.x, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->target_.y, targetDestinationPos.y, adjustedTimeValue);
	IFE::Complement(actionCamera_->transform_->target_.z, targetDestinationPos.z, adjustedTimeValue);
}

void PlayerAction::CameraRot()
{
	//回転速度
	const float rotSpeed = 80.0f;

#pragma region キーボード
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
#pragma endregion キーボード

#pragma region コントローラー

	cameraAngle_ += {-IFE::Input::GetRYAnalog(controllerRange_) * rotSpeed * IFE::IFETime::sDeltaTime_, IFE::Input::GetRXAnalog(controllerRange_)* rotSpeed* IFE::IFETime::sDeltaTime_};

#pragma endregion コントローラー

	//縦回転限界処理
	cameraAngle_.x = std::clamp(cameraAngle_.x, -60.0f, 60.0f);
}