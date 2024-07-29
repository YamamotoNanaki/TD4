#include "Player.h"
#include"Transform.h"
#include "Input.h"
#include "Collider.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include"ObjectManager.h"
#include"CameraManager.h"
#include"SpriteManager.h"
#include"DronePostEffect.h"
#include "Sound.h"

void Player::Initialize()
{
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	drone_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>();
	IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");

	enemyHilight_ = IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting");
	dronePostEffect_ = IFE::PostEffectManager::Instance()->GetPostEffect("DronePostEffect");
	dynamic_cast<EnemyHighlighting*>(IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting"))->droneHighlightingDistance_ = droneHighlightingDistance_;

	ui_->UIChange(modeFlag_);

	auto droneRecoveryUIPtr = IFE::SpriteManager::Instance()->GetSpritePtr("droneRecoveryUI")->GetComponent<DroneRecoveryUI>();
	droneRecoveryUI_ = droneRecoveryUIPtr;

	pose_ = IFE::ObjectManager::Instance()->GetObjectPtr("PoseMenu")->GetComponent<PoseMenu>();

	transform_->position_ = { 0,0,0 };
	objectPtr_->DrawFlag_ = false;
	IFE::Sound::Instance()->LoadMP3("droneChange");
	IFE::Sound::Instance()->SetVolume("droneChange", 80);
	IFE::Sound::Instance()->LoadMP3("droneClose");
	IFE::Sound::Instance()->SetVolume("droneClose", 80);
	IFE::Sound::Instance()->LoadMP3("drone");
	IFE::Sound::Instance()->SetVolume("drone", 50);
	IFE::Sound::Instance()->LoadMP3("droneRecovery");
	IFE::Sound::Instance()->SetVolume("droneRecovery", 60);
	IFE::Sound::Instance()->LoadMP3("droneBack");
	IFE::Sound::Instance()->SetVolume("droneBack", 120);
}

void Player::Update()
{
	if (pose_->GetPoseFlag() == false)
	{
		ChangeMode();

		DroneRecovery();

		if (modeFlag_ == false)
		{
			if (IFE::Sound::Instance()->GetPlayStatus("drone")) {
				IFE::Sound::Instance()->StopSound("drone");
			}
			action_->MoveUpdate();
			if (!droneRecoveryFlag_ && IFE::Sound::Instance()->GetPlayStatus("droneRecovery")) {
				IFE::Sound::Instance()->StopSound("droneRecovery");
			}
		}
		else
		{
			if (droneRecoveryFlag_ == false)
			{
				if (!IFE::Sound::Instance()->GetPlayStatus("drone")) {
					IFE::Sound::Instance()->SoundPlay("drone", false, true);
				}
				drone_->MoveUpdate();
			}
		}
		oldIsDamageFlag_ = action_->GetIsHit();
	}
}

void Player::Draw()
{
}

void Player::Finalize()
{
	delete droneRecoveryUI_;
}

void Player::OnColliderHit(IFE::ColliderCore collider)
{
}

void Player::DroneBreak()
{
	if (IFE::Sound::Instance()->GetPlayStatus("drone")) {
		IFE::Sound::Instance()->StopSound("drone");
	}
	drone_->SetIsDroneSurvival(false);
	enemyHilight_->updateFlag_ = false;
	dynamic_cast<DronePostEffect*>(dronePostEffect_)->droneFlag_ = false;
	if (modeFlag_ == true)
	{
		modeFlag_ = false;
		IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
	}
	drone_->SetDrawFlag(false);
	droneRecoverytime_ = 0.0f;
	droneRecoveryUI_->SetDrawFlag(false);
	droneRecoveryFlag_ = false;
	IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = false;
}

bool Player::GetMode()
{
	return modeFlag_;
}

void Player::SetMode(bool flag)
{
	modeFlag_ = flag;
}

void Player::ChangeMode()
{
	if (action_->GetAttackFlag())return;

	if ((!ccp_.cameraChange && IFE::Input::GetKeyTrigger(IFE::Key::Y) || IFE::Input::PadTrigger(IFE::PADCODE::Y)) || (modeFlag_ == true && oldIsDamageFlag_ == false && action_->GetIsHit() == true))
	{
		if (modeFlag_) {
			IFE::Sound::Instance()->SoundPlay("droneClose", false, true);
		}
		else {
			IFE::Sound::Instance()->SoundPlay("droneChange", false, true);
		}
		ccp_.cameraChange = true;
		ccp_.cameraChangeTimer = 0;
		ccp_.change = false;
	}
	if (ccp_.cameraChange)
	{
		ccp_.cameraChangeTimer += IFE::IFETime::sDeltaTime_;
		if (!ccp_.change && ccp_.cameraChangeTimer >= ccp_.changeTime)
		{
			ccp_.change = true;
			if (modeFlag_ == false)
			{
				//アクションからドローンモードへ
				modeFlag_ = true;
				drone_->SetDrawFlag(false);
				IFE::CameraManager::Instance()->SetActiveCamera("DroneCamera");
				action_->SetAnimation("drone");

				dynamic_cast<DronePostEffect*>(dronePostEffect_)->droneFlag_ = true;
				if (drone_->GetIsDroneSurvival() == false)
				{
					IFE::Float3 pos = action_->GetPos();
					pos.y += 3.0f;
					drone_->SetPos(pos);
					drone_->SetRotY(action_->GetRotY());
					drone_->SetIsDroneSurvival(true);
					enemyHilight_->updateFlag_ = true;
					IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = true;
				}
			}
			else
			{
				//ドローンからアクションモードへ
				modeFlag_ = false;
				drone_->SetDrawFlag(drone_->GetIsDroneSurvival());
				IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
				if (IFE::Input::GetKeyPush(IFE::Key::W) || IFE::Input::GetKeyPush(IFE::Key::A) || IFE::Input::GetKeyPush(IFE::Key::S) || IFE::Input::GetKeyPush(IFE::Key::D) || IFE::Input::GetLAnalog().x != 0.0f && IFE::Input::GetLAnalog().y != 0.0f)
				{
					action_->SetIsWalk(true);
					action_->SetAnimation("walk");
				}
				else
				{
					action_->SetIsWalk(false);
					action_->SetAnimation("standBy");
				}
				dynamic_cast<DronePostEffect*>(dronePostEffect_)->droneFlag_ = false;				
			}
			//UI表示切替
			ui_->UIChange(modeFlag_);
		}
		if (ccp_.cameraChangeTimer >= ccp_.cameraChangeMaxTime)
		{
			ccp_.cameraChange = false;
		}
	}
}

void Player::DroneRecovery()
{
	if (drone_->GetIsDroneSurvival() == true)
	{
		if (droneRecoveryFlag_ == false && (IFE::Input::PadTrigger(IFE::PADCODE::DOWN) || IFE::Input::GetKeyTrigger(IFE::Key::R)))
		{
			droneRecoveryFlag_ = true;
			droneRecoveryUI_->SetDrawFlag(true);
		}
	}

	if (droneRecoveryFlag_ == true)
	{
		if (!IFE::Sound::Instance()->GetPlayStatus("droneRecovery")) {
			IFE::Sound::Instance()->SoundPlay("droneRecovery", false, true);
		}
		droneRecoveryUI_->Recovery(droneRecoverytime_, maxDroneRecoverytime_);
		droneRecoverytime_ += IFE::IFETime::sDeltaTime_;
		if (droneRecoverytime_ > maxDroneRecoverytime_)
		{
			IFE::Sound::Instance()->SoundPlay("droneBack", false, false);
			DroneBreak();
		}
	}
}

#ifdef EditorMode
#include "ImguiManager.h"
void Player::ComponentDebugGUI()
{
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->DragFloatGUI(&droneHighlightingDistance_, "Highlighting Distance");
}
void Player::OutputComponent(nlohmann::json& json)
{
	json["Highlighting"] = droneHighlightingDistance_;
}
#endif
void Player::LoadingComponent(nlohmann::json& json)
{
	droneHighlightingDistance_ = json["Highlighting"];
}
