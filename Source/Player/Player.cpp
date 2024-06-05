#include "Player.h"
#include"Transform.h"
#include "Input.h"
#include "Collider.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include"ObjectManager.h"
#include"CameraManager.h"
#include"SpriteManager.h"
#include"PostEffectManager.h"
#include"EnemyHighlighting.h"

void Player::Initialize()
{
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	drone_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>();

	IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");

	ChangeUI();

	transform_->position_ = { 0,0,0 };
	objectPtr_->DrawFlag_ = false;
	dynamic_cast<EnemyHighlighting*>(IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting"))->droneHighlightingDistance_ = droneHighlightingDistance_;
}

void Player::Update()
{
	ChangeMode();

	if (modeFlag_ == false)
	{
		action_->MoveUpdate();
	}
	else
	{
		drone_->MoveUpdate();
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

bool Player::GetMode()
{
	return modeFlag_;
}

void Player::ChangeMode()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Y) || IFE::Input::PadTrigger(IFE::PADCODE::Y))
	{
		if (modeFlag_ == false)
		{
			//ドローンモード
			modeFlag_ = true;
			drone_->SetDrawFlag(false);
			IFE::CameraManager::Instance()->SetActiveCamera("DroneCamera");
			auto ptr = IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting");
			ptr->drawFlag_ = true;
			ptr->updateFlag_ = true;
			if (isDrone_ == false)
			{
				drone_->SetPos(action_->GetPos());
				isDrone_ = true;
			}
		}
		else
		{
			//アクションモード
			modeFlag_ = false;
			drone_->SetDrawFlag(true);
			IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
			auto ptr = IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting");
			ptr->drawFlag_ = false;
			ptr->updateFlag_ = false;
		}
		//UI表示切替
		ChangeUI();
	}
}

void Player::ChangeUI()
{
	if (modeFlag_ == true)
	{
		//ドローンモードのUI
		IFE::SpriteManager::Instance()->GetSpritePtr("flame")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Down")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Up")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeDrone")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("SpeedChange")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("RT")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("LT")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("PauseText")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("PauseDrone")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("PauseTextDrone")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("YDrone")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Attack")->drawFlag_ = false;
	}
	else
	{
		//アクションモードのUI
		IFE::SpriteManager::Instance()->GetSpritePtr("flame")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Down")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Up")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeDrone")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("SpeedChange")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("RT")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("LT")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("PauseText")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("PauseDrone")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("PauseTextDrone")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("YDrone")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Attack")->drawFlag_ = true;
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
