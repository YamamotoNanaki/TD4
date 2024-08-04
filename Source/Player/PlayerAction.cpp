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
#include "IFEEffekseerManager.h"
#include "Collider.h"
#include "TextureManager.h"
#include "Material.h"

void PlayerAction::Initialize()
{
	transform_->position_.y = 2.0f;

	camera_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerActionCameraObject")->GetComponent<PlayerActionCamera>();
	camera_->CameraInitialize(transform_->position_);

	//HP
	auto hpPtr = IFE::SpriteManager::Instance()->GetSpritePtr("playerHp")->GetComponent<IFE::PlayerHp>();
	playerHp_ = hpPtr;
	playerHp_->SetHp(hp_);

	auto deathPtr = IFE::SpriteManager::Instance()->GetSpritePtr("gameOverFade")->GetComponent<DeathAnimation>();
	deathAnimation_ = deathPtr;

	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("PlayerAttack", IFE::ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<PlayerAttack>();

	playerAttack_ = ptr->GetComponent<PlayerAttack>();
	playerAttack_->SetName("player");

	enemyManager_ = IFE::ObjectManager::Instance()->GetObjectPtr("EnemyManager")->GetComponent<IFE::EnemyManager>();

	ani_ = objectPtr_->GetComponent<IFE::Animator>();
	ani_->SetAnimation("standBy");//待機モーションに変える

	moveSpeed_ = normalMoveSpeed_;

	//sound
	IFE::Sound::Instance()->LoadWave("walk",IFE::Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("walk", 60);
	IFE::Sound::Instance()->LoadWave("attack", IFE::Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("attack", 60);

	auto texm = IFE::TextureManager::Instance();
	for (auto& mat : objectPtr_->GetComponent<IFE::Material>()->childMaterials_)
	{
		if (mat.first.name == "PLayer")
		{
			mat.first.tex = texm->GetTexture("enemy");
		}
		if (mat.first.name == "Knife")
		{
			mat.first.tex = texm->GetTexture("Knife");
		}
		if (mat.first.name == "Helmet")
		{
			mat.first.tex = texm->GetTexture("helmet");
		}
		mat.first.ambient = { 0.3f,0.3f ,0.3f };
		mat.first.diffuse = { 0.7f,0.7f ,0.7f };
		mat.first.specular = { 1.0f,1.0f ,1.0f };
	}
}

void PlayerAction::Update()
{
	if (clearFlag_ == true)return;

	if (ani_->GetAnimation() == "damage" || ani_->GetAnimation() == "squatDamage" || ani_->GetAnimation() == "downFront" || ani_->GetAnimation() == "downBack")
	{
		ani_->loop_ = false;
	}
	else
	{
		ani_->loop_ = true;
	}

	if (deathAnimationFlag_ == false && ani_->animEnd_ == false)
	{
		//hitcool
		if (isHit_ == true)
		{
			playerHp_->ScaleCalc(hp_, 1, hitTime_, HIT_COOLTIME);
			hitTime_ -= IFE::IFETime::sDeltaTime_;
			if (hitTime_ <= 0) {
				isHit_ = false;

				if (hp_ <= 0)
				{
					deathAnimationFlag_ = true;
				}
				else
				{
					if (crouchFlag_ == false)
					{
						ani_->SetAnimation("standBy");
						if ((IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f) || IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D))
						{
							ani_->SetAnimation("walk");
						}
					}
					else
					{
						ani_->SetAnimation("squatStandBy");
						if ((IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f) || IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D))
						{
							ani_->SetAnimation("squatWalk");
						}
					}
				}
			}
		}

		if (hp_ > 0)
		{
			if (IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f)
			{
				if (ani_->GetAnimation() == "walk")
				{
					ani_->animSpeed_ = sqrt(IFE::Input::GetLXAnalog(controllerRange_) * IFE::Input::GetLXAnalog(controllerRange_) + IFE::Input::GetLYAnalog(controllerRange_) * IFE::Input::GetLYAnalog(controllerRange_)) * 2.0f;
				}
				else if (ani_->GetAnimation() == "squatWalk")
				{
					ani_->animSpeed_ = sqrt(IFE::Input::GetLXAnalog(controllerRange_) * IFE::Input::GetLXAnalog(controllerRange_) + IFE::Input::GetLYAnalog(controllerRange_) * IFE::Input::GetLYAnalog(controllerRange_)) * 2.0f;
				}
				else
				{
					ani_->animSpeed_ = 1.0f;
				}
			}
			else
			{
				if (ani_->GetAnimation() == "walk")
				{
					ani_->animSpeed_ = 2.0f;
				}
				else if (ani_->GetAnimation() == "squatWalk")
				{
					ani_->animSpeed_ = 2.0f;
				}
				else
				{
					ani_->animSpeed_ = 1.0f;
				}
			}
		}
	}

	if (hp_ <= 0)
	{
		deathAnimation_->SetDeathAnimationFlag(true);
	}
}

void PlayerAction::Draw()
{
}

void PlayerAction::Finalize()
{

}

void PlayerAction::DecHp(bool isBack_)
{
#ifdef EditorMode
	if (cheatFlag_)return;
#endif
	if (isHit_ == false) {
		if (hp_ > 0)
		{
			hp_--;
		}
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
		if (hp_ > 0)
		{
			if (crouchFlag_ == false)
			{
				ani_->SetAnimation("damage", false);
			}
			else
			{
				ani_->SetAnimation("squatDamage");
			}
			auto e = IFE::IFEEffekseerManager::Instance()->GetEffekseer("LowHPBlood");
			e->Play(&transform_->position_, nullptr, nullptr, 0, 0, { 0.5,0.5,0.5 });
		}
		else
		{
			if (crouchFlag_ == false)
			{
				//敵が後ろ
				if (isBack_)
				{
					ani_->SetAnimation("downFront");
				}
				//敵が前
				else
				{
					ani_->SetAnimation("downBack");
				}
			}
			else
			{
				ani_->SetAnimation("squatDamage");//しゃがみ死亡モーション
			}
			auto e = IFE::IFEEffekseerManager::Instance()->GetEffekseer("LowHPBlood");
			e->Play(&transform_->position_, nullptr, nullptr, 0, 0, { 0.5,0.5,0.5 });
		}
	}
}

void PlayerAction::MoveUpdate()
{
	if (deathAnimationFlag_ == false)
	{
		camerafrontVec_ = { camera_->GetCamera()->transform_->target_.x - camera_->GetPos().x,0.0f,camera_->GetCamera()->transform_->target_.z - camera_->GetPos().z };
		camerafrontVec_.Normalize();
		if (crouchAnimationFlag_ == false)
		{
			if (attackFlag_ == false)
			{
				Rotation();
				Move();
				Crouch();
				objectPtr_->SetColor({ 1,1,1,1 });
			}
			else
			{
				objectPtr_->SetColor({ 1,0,0,1 });
			}
			//攻撃のUI等の兼ね合いで個別記入
			Attack();
		}
		CrouchAnimation();

		camera_->CameraUpdate(transform_->position_);
	}
}

void PlayerAction::Move()
{
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

	IsWalk();

#pragma region キーボード
	if (IFE::Input::GetKeyPush(IFE::Key::A))
	{
		transform_->position_ += rightVec_ * moveSpeed_ * IFE::IFETime::sDeltaTime_;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::D))
	{
		transform_->position_ -= rightVec_ * moveSpeed_ * IFE::IFETime::sDeltaTime_;
	}if (IFE::Input::GetKeyPush(IFE::Key::W))
	{
		transform_->position_ += frontVec_ * moveSpeed_ * IFE::IFETime::sDeltaTime_;
	}if (IFE::Input::GetKeyPush(IFE::Key::S))
	{
		transform_->position_ -= frontVec_ * moveSpeed_ * IFE::IFETime::sDeltaTime_;
	}
	if (IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) ||
		IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D)) {
		if (!IFE::Sound::Instance()->GetPlayStatus("walk")) {
			IFE::Sound::Instance()->SoundPlay("walk", false, true);
		}
	}
#pragma endregion キーボード

#pragma region コントローラー
	//入力されている方向
	targetVec_ = { IFE::Input::GetLXAnalog(controllerRange_) ,0,IFE::Input::GetLYAnalog(controllerRange_) };

	if (targetVec_.x != 0 || targetVec_.z != 0)
	{
		ApproachTarget(actualFrontVec_.x, targetVec_.x, 0.05f);
		ApproachTarget(actualFrontVec_.z, targetVec_.z, 0.05f);
		transform_->position_ -= actualFrontVec_.x * rightVec_ * moveSpeed_ * IFE::IFETime::sDeltaTime_;
		transform_->position_ += actualFrontVec_.z * frontVec_ * moveSpeed_ * IFE::IFETime::sDeltaTime_;
		if (IFE::Sound::Instance()->GetPlayStatus("walk") == false) {
			IFE::Sound::Instance()->SoundPlay("walk", false, true);
		}
	}
	//重力
	if (!objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->onGround_)
	{
		transform_->position_.y -= 4.9f * IFE::IFETime::sDeltaTime_;
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

const IFE::Vector3 PlayerAction::GetActualFrontVec()
{
	return actualFrontVec_;
}

const IFE::Vector3 PlayerAction::GetRot()
{
	return transform_->rotation_;
}

const float PlayerAction::GetRotY()
{
	return rotY_;
}

void PlayerAction::SetAnimation(std::string name)
{
	ani_->SetAnimation(name);
}

void PlayerAction::SetIsWalk(bool flag)
{
	isWalk_ = flag;
}

void PlayerAction::SetClearFlag(bool flag)
{
	clearFlag_ = flag;
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
		ApproachTarget(rotY_, targetAngle, 10.0f);
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
	AutoAim();
	AttackUI();
	if (attackFlag_ == false)
	{
		if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::X))
		{
			if (crouchFlag_ == false)
			{
				if (playerAttack_->GetIsBackAttack() == false)
				{
					ani_->SetAnimation("knifeAttack");
				}
				else
				{
					ani_->SetAnimation("backKnifeAttack");
					slowFlag_ = true;
				}
			}
			else
			{
				if (playerAttack_->GetIsBackAttack() == false)
				{
					ani_->SetAnimation("squatKnifeAttack");
				}
				else
				{
					ani_->SetAnimation("squatKnifeAttack");//しゃがみワンパン攻撃モーションに変える
					slowFlag_ = true;
				}
			}
			attackFlag_ = true;
			playerAttack_->SetAttackFlag(attackFlag_);
		}
	}

	if (attackFlag_ == true)
	{
		if (attackTimer_ > maxAttackTime_ - IFE::IFETime::sDeltaTime_)
		{
			if (isAttack_ == false) {
				IFE::Sound::Instance()->SoundPlay("attack", false, true);
			}
			isAttack_ = true;
#ifdef DEBUG
			playerAttack_->objectPtr_->DrawFlag_ = true;
#endif // DEBUG
		}

		if (attackTimer_ > maxAttackAnimationTime_)
		{
			attackFlag_ = false;
			playerAttack_->SetAttackFlag(attackFlag_);
			isAttack_ = false;
			attackTimer_ = 0;
#ifdef DEBUG
			playerAttack_->objectPtr_->DrawFlag_ = false;
#endif // DEBUG
			slowFlag_ = false;
			IFE::IFETime::sTimeScale_ = 1.0f;
			slowEaseTime_ = 0;
			nowGameTimeScale_ = 1;
			if (crouchFlag_ == false)
			{
				if ((IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f) || IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D))
				{
					ani_->SetAnimation("walk");
				}
				else
				{
					ani_->SetAnimation("standBy");
				}
			}
			else
			{
				if ((IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f) || IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D))
				{
					ani_->SetAnimation("squatWalk");
				}
				else
				{
					ani_->SetAnimation("squatStandBy");
				}
			}
		}

		attackTimer_ += IFE::IFETime::sDeltaTime_;
		}

	playerAttack_->SetIsAttack(isAttack_);
	SlowMotion();
	}

void PlayerAction::SlowMotion()
{
	if (slowFlag_)
	{
		if (attackTimer_ >= 0.6 && slowEaseTime_ < 0.3)
		{
			slowEaseTime_ += min(IFE::IFETime::sNoScaleDeltaTime_, 0.3f);
			nowGameTimeScale_ = IFE::EaseOutCirc(slowEaseTime_, 1, minSlowSpeed_, 0.3f);
		}
		else if (attackTimer_ >= 0.8 && slowEaseTime_ < 0.5)
		{
			slowEaseTime_ += IFE::IFETime::sNoScaleDeltaTime_;
			nowGameTimeScale_ = IFE::EaseOutCirc(slowEaseTime_ - 0.3f, minSlowSpeed_, 1, 0.2f);
		}
		else
		{
			nowGameTimeScale_ = minSlowSpeed_;
		}
		IFE::IFETime::sTimeScale_ = nowGameTimeScale_;
	}
}

void PlayerAction::AttackUI()
{
	float minDistance = 5.0f;
	closestEnemy = nullptr;
	for (auto& enemys : enemyManager_->GetEnemyList())
	{
		if (sqrt((transform_->position_.y - enemys->GetPos().y) * (transform_->position_.y - enemys->GetPos().y)) > 2.0f)
		{
			continue;
		}

		float distance = sqrt((enemys->GetPos().x - transform_->position_.x) * (enemys->GetPos().x - transform_->position_.x) + (enemys->GetPos().z - transform_->position_.z) * (enemys->GetPos().z - transform_->position_.z));
		if (distance < minDistance)
		{
			minDistance = distance;
			closestEnemy = enemys;
		}
	}

	if (attackFlag_ == false)
	{
		if (closestEnemy != nullptr && IFE::Collision::CheckCircle({ {transform_->position_.x,transform_->position_.z},5.0f }, { {closestEnemy->GetPos().x,closestEnemy->GetPos().z},1.0f }))
		{
			isXBottonUI_ = true;
		}
		else
		{
			isXBottonUI_ = false;
			playerAttack_->SetIsBackAttack(false);
		}
	}
	else
	{
		isXBottonUI_ = false;
	}

	IFE::SpriteManager::Instance()->GetSpritePtr("attackUI")->drawFlag_ = isXBottonUI_;
}

void PlayerAction::ApproachTarget(float& current, float target, float step)
{
	if (std::abs(current - target) > 180)
	{
		if (target > current)
		{
			target -= 360;
		}
		else
		{
			target += 360;
		}
	}
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
	if (isXBottonUI_ == true)
	{
		IFE::Vector3 frontVec = closestEnemy->transform_->position_ - transform_->transform_->position_;
		playerAttack_->objectPtr_->transform_->position_ =
		{ transform_->position_.x + frontVec.x,
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
	else
	{
		playerAttack_->objectPtr_->transform_->position_ =
		{ transform_->position_.x + frontVec_.x,
			transform_->position_.y + frontVec_.y,
			transform_->position_.z + frontVec_.z
		};
	}
}

void PlayerAction::IsWalk()
{
	if ((IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f) || IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D))
	{
		isWalk_ = true;
	}
	else
	{
		isWalk_ = false;
	}

	if (oldIsWalk_ == false && isWalk_ == true)
	{
		if (crouchFlag_ == false)
		{
			ani_->SetAnimation("walk");
		}
		else
		{
			ani_->SetAnimation("squatWalk");
		}
	}
	if (oldIsWalk_ == true && isWalk_ == false)
	{
		if (crouchFlag_ == false)
		{
			ani_->SetAnimation("standBy");
		}
		else
		{
			ani_->SetAnimation("squatStandBy");
		}
	}
	oldIsWalk_ = isWalk_;
}

void PlayerAction::Crouch()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::LSHIFT) || IFE::Input::PadTrigger(IFE::PADCODE::B))
	{
		crouchAnimationFlag_ = true;
		if (crouchFlag_ == false)
		{
			moveSpeed_ = crouchMoveSpeed_;
			ani_->SetAnimation("squat");//しゃがみ
		}
		else
		{
			moveSpeed_ = normalMoveSpeed_;
			ani_->SetAnimation("standUp");//立ち
		}
	}
}

void PlayerAction::CrouchAnimation()
{
	if (crouchAnimationFlag_ == true)
	{
		if (crouchAnimationTimer_ > maxCrouchTime_)
		{
			crouchAnimationTimer_ = 0.0f;
			crouchFlag_ = !crouchFlag_;
			if (crouchFlag_ == false)
			{
				ani_->SetAnimation("standBy");
				if (IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D) || IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f)
				{
					ani_->SetAnimation("walk");
				}
			}
			else
			{
				ani_->SetAnimation("squatStandBy");
				if (IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D) || IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f)
				{
					ani_->SetAnimation("squatWalk");
				}
			}
			crouchAnimationFlag_ = false;
		}

		crouchAnimationTimer_ += IFE::IFETime::sDeltaTime_;
	}
}

void PlayerAction::SetIsXBottonUI(bool flag)
{
	isXBottonUI_ = flag;
}

#ifdef EditorMode
#include "ImguiManager.h"
void PlayerAction::ComponentDebugGUI()
{
	IFE::ImguiManager::Instance()->CheckBoxGUI(&cheatFlag_, U8("チート"));
}

#endif