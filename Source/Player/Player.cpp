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

void Player::Initialize()
{
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	drone_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>();
	IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");

	enemyHilight_ = IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting");
	dronePostEffect_ = IFE::PostEffectManager::Instance()->GetPostEffect("DronePostEffect");
	dynamic_cast<EnemyHighlighting*>(IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting"))->droneHighlightingDistance_ = droneHighlightingDistance_;

	ui_->UIChange(modeFlag_);

	transform_->position_ = { 0,0,0 };
	objectPtr_->DrawFlag_ = false;
}

void Player::Update()
{
	ChangeMode();

	DroneRecovery();

	if (modeFlag_ == false)
	{
		action_->MoveUpdate();
	}
	else
	{
		if (droneRecoveryFlag_ == false)
		{
			drone_->MoveUpdate();
		}
	}
}

void Player::Draw()
{
}

void Player::Finalize()
{
}

void Player::OnColliderHit(IFE::ColliderCore collider)
{
}

void Player::DroneBreak()
{
	drone_->SetIsDroneSurvival(false);
	enemyHilight_->updateFlag_ = false;
	dynamic_cast<DronePostEffect*>(dronePostEffect_)->droneFlag_ = false;
	if (modeFlag_ == true)
	{
		modeFlag_ = false;
		IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
	}
	drone_->SetDrawFlag(false);
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
	if (IFE::Input::GetKeyTrigger(IFE::Key::Y) || IFE::Input::PadTrigger(IFE::PADCODE::Y))
	{
		if (modeFlag_ == false)
		{
			//アクションからドローンモードへ
			modeFlag_ = true;
			drone_->SetDrawFlag(false);
			IFE::CameraManager::Instance()->SetActiveCamera("DroneCamera");

			if (drone_->GetIsDroneSurvival() == false)
			{
				IFE::Float3 pos = action_->GetPos();
				pos.y += 3.0f;
				drone_->SetPos(pos);
				drone_->SetRotY(action_->GetRotY());
				drone_->SetIsDroneSurvival(true);
				enemyHilight_->updateFlag_ = true;
				dynamic_cast<DronePostEffect*>(dronePostEffect_)->droneFlag_ = true;
			}
		}
		else
		{
			//ドローンからアクションモードへ
			modeFlag_ = false;
			drone_->SetDrawFlag(drone_->GetIsDroneSurvival());
			IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
		}
	}
	//UI表示切替(毎フレームやるの良くない)
	ui_->UIChange(modeFlag_);
}

void Player::DroneRecovery()
{
	if (droneRecoveryFlag_ == false && IFE::Input::PadTrigger(IFE::PADCODE::DOWN))
	{
		droneRecoveryFlag_ = true;
	}

	if (droneRecoveryFlag_ == true)
	{
		droneRecoverytime_ += /*IFE::IFETime::sDeltaTime_*/1;
		if (droneRecoverytime_ > maxDroneRecoverytime_)
		{
			DroneBreak();
			droneRecoveryFlag_ = false;
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
