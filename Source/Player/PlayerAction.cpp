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
#include"Collision.h"
#include "Sound.h"

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
	playerAttack_->SetName("player");

	enemyManager_ = IFE::ObjectManager::Instance()->GetObjectPtr("EnemyManager")->GetComponent<IFE::EnemyManager>();

	ani_ = objectPtr_->GetComponent<IFE::Animator>();
	ani_->SetAnimation("walk");
	//sound
	IFE::Sound::Instance()->LoadWave("walk");
	IFE::Sound::Instance()->SetVolume("walk", 50);
	IFE::Sound::Instance()->LoadWave("attack");
	IFE::Sound::Instance()->SetVolume("attack", 50);
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
	camerafrontVec_ = { camera_->GetCamera()->transform_->target_.x - camera_->GetPos().x,0.0f,camera_->GetCamera()->transform_->target_.z - camera_->GetPos().z };
	camerafrontVec_.Normalize();
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

	//正面ベクトルの作成
	frontVec_ = transform_->position_ - camera_->GetPos();
	frontVec_.Normalize();
	//仮ベクトル
	temporaryVec_ = { 0,1,0 };
	//右ベクトルの作成
	rightVec_ = frontVec_.Cross(temporaryVec_);
	rightVec_.Normalize();

	//今回はY軸の動きは無くて良い
	frontVec_.y = 0.0f;
	rightVec_.y = 0.0f;
	frontVec_.Normalize();
	rightVec_.Normalize();

	//if (objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->onGround_ == false)
	//{
	//	transform_->position_.y = -4.9f * IFE::IFETime::sDeltaTime_;
	//}

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
	//if (IFE::Input::GetKeyPush(IFE::Key::W)|| IFE::Input::GetKeyPush(IFE::Key::A)||
	//	IFE::Input::GetKeyPush(IFE::Key::S)|| IFE::Input::GetKeyPush(IFE::Key::D)) {
	//	IFE::Sound::Instance()->SoundPlay("walk", false, true);
	//}
#pragma endregion キーボード

#pragma region コントローラー
	//入力されている方向
	targetVec_ = { IFE::Input::GetLXAnalog(controllerRange_) ,0,IFE::Input::GetLYAnalog(controllerRange_) };

	if (targetVec_.x != 0 || targetVec_.z != 0)
	{
		approachTarget(actualFrontVec_.x, targetVec_.x, 0.05f);
		approachTarget(actualFrontVec_.z, targetVec_.z, 0.05f);
		transform_->position_ -= actualFrontVec_.x * rightVec_ * speed * IFE::IFETime::sDeltaTime_;
		transform_->position_ += actualFrontVec_.z * frontVec_ * speed * IFE::IFETime::sDeltaTime_;
		IFE::Sound::Instance()->SoundPlay("walk", false, true);
	}
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
		transform_->rotation_.y = rotY_;
	}
#pragma endregion キーボード

#pragma region コントローラー
	float lx = IFE::Input::GetLXAnalog(controllerRange_);
	float ly = IFE::Input::GetLYAnalog(controllerRange_);

	if (lx == 0 || ly == 0)
	{
		//方向ベクトルの角度+コントローラーの角度
		float targetAngle = IFE::ConvertToDegrees(std::atan2(camerafrontVec_.x, camerafrontVec_.z) + std::atan2(actualFrontVec_.x, actualFrontVec_.z));
		approachTarget(rotY_, targetAngle, 10.0f);
		if (!(lx == 0 && ly == 0))
		{
			transform_->rotation_.y = rotY_;
		}
	}
	else
	{
		//方向ベクトルの角度+コントローラーの角度
		rotY_ = IFE::ConvertToDegrees(std::atan2(camerafrontVec_.x, camerafrontVec_.z) + std::atan2(actualFrontVec_.x, actualFrontVec_.z));
		transform_->rotation_.y = rotY_;
	}

#pragma endregion コントローラー
}

void PlayerAction::Attack()
{
	AttackUI();
	AutoAim();
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::X))
	{
		IFE::Sound::Instance()->SoundPlay("attack", false, true);
		if (playerAttack_->GetIsBackAttack() == false)
		{
			ani_->SetAnimation("walk");
		}
		else
		{
			ani_->SetAnimation("backKnifeAttack");
		}
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

void PlayerAction::AttackUI()
{
	float minDistance = 5.0f;
	closestEnemy = nullptr;
	for (auto& enemys : enemyManager_->GetEnemyList())
	{
		float distance = sqrt((enemys->GetPos().x - transform_->position_.x) * (enemys->GetPos().x - transform_->position_.x) + (enemys->GetPos().y - transform_->position_.z) * (enemys->GetPos().y - transform_->position_.z));
		if (distance < minDistance) {
			minDistance = distance;
			closestEnemy = enemys;
		}
	}

	if (attackFlag_ == false)
	{
		if (closestEnemy != nullptr && IFE::Collision::CheckCircle({ {transform_->position_.x,transform_->position_.z},5.0f }, { closestEnemy->GetPos(),1.0f }))
		{
			isAttackUI_ = true;
		}
		else
		{
			isAttackUI_ = false;
		}
	}
	else
	{
		isAttackUI_ = false;
	}

	IFE::SpriteManager::Instance()->GetSpritePtr("attackUI")->drawFlag_ = isAttackUI_;
}

void PlayerAction::approachTarget(float& current, float target, float step)
{
	if (std::abs(current - target) < step) {
		current = target; // 目標値にほぼ達した場合、目標値に設定
	}
	else if (current < target) {
		current += step; // 現在値が目標値より小さい場合、増加
	}
	else {
		current -= step; // 現在値が目標値より大きい場合、減少
	}
}

void PlayerAction::AutoAim()
{
	if (isAttackUI_ == true)
	{
		IFE::Vector3 frontVec = closestEnemy->transform_->position_ - transform_->transform_->position_;
		playerAttack_->objectPtr_->transform_->position_ =
		{	transform_->position_.x + frontVec.x,
			transform_->position_.y + frontVec.y,
			transform_->position_.z + frontVec.z
		};
		frontVec.y = 0.0f;
		frontVec.Normalize();

		if (attackFlag_ == true)
		{
			rotY_ = IFE::ConvertToDegrees(std::atan2(frontVec.x, frontVec.z));
			transform_->rotation_.y = rotY_;
		}
	}
}