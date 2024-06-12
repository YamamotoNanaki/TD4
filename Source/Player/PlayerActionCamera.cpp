#include "PlayerActionCamera.h"
#include"Transform.h"
#include "Collider.h"
#include "Input.h"
#include "IFETime.h"
#include"Object3D.h"

void PlayerActionCamera::Initialize()
{
	objectPtr_->DrawFlag_ = false;
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
}

void PlayerActionCamera::Update()
{
	if (!ColliderHitFlag_)
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
	myCollider;
	if (hitCollider->attribute_ == static_cast<uint16_t>(IFE::Attribute::LANDSHAPE))
	{
		//当たったときの距離の代入
		/*IFE::Vector3 vec = myCollider->interPoint_ - IFE::Vector3(transform_->position_);
		if (distance_ > vec.Length())
		{
			distance_ = vec.Length();
			ColliderHitFlag_ = true;
		}*/
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
	//視点の移動先の座標
	IFE::Vector3 eyeDestinationPos =
	{
		playerPos.x + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		playerPos.y + cameraYAdd_ + distance_ * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		playerPos.z + distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//視点の補間移動
	IFE::Complement(transform_->position_.x, eyeDestinationPos.x, adjustedTimeValue_);
	IFE::Complement(transform_->position_.y, eyeDestinationPos.y, adjustedTimeValue_);
	IFE::Complement(transform_->position_.z, eyeDestinationPos.z, adjustedTimeValue_);

	actionCamera_->transform_->eye_ = transform_->position_;

	//注視点の移動先の座標
	IFE::Vector3 targetDestinationPos =
	{
		playerPos.x - distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * sinf(IFE::ConvertToRadians(cameraAngle_.y)),
		playerPos.y - distance_ * sinf(IFE::ConvertToRadians(cameraAngle_.x)),
		playerPos.z - distance_ * cosf(IFE::ConvertToRadians(cameraAngle_.x)) * cosf(IFE::ConvertToRadians(cameraAngle_.y))
	};

	//注視点の補間移動
	IFE::Complement(actionCamera_->transform_->target_.x, targetDestinationPos.x, adjustedTimeValue_);
	IFE::Complement(actionCamera_->transform_->target_.y, targetDestinationPos.y, adjustedTimeValue_);
	IFE::Complement(actionCamera_->transform_->target_.z, targetDestinationPos.z, adjustedTimeValue_);
}

void PlayerActionCamera::CameraRot()
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

void PlayerActionCamera::CameraUpdate(const IFE::Vector3& playerPos)
{
	//カメラの回転
	CameraRot();

	//カメラ補間
	CameraMove(playerPos);
}