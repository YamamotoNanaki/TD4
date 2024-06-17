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

	ui_->UIChange(modeFlag_);

	transform_->position_ = { 0,0,0 };
	objectPtr_->DrawFlag_ = false;
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
	dynamic_cast<EnemyHighlighting*>(IFE::PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting"))->droneHighlightingDistance_ = droneHighlightingDistance_;
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
			if (drone_->GetIsDroneSurvival() == false)
			{
				IFE::Float3 pos = action_->GetPos();
				pos.y += 3.0f;
				drone_->SetPos(pos);
				drone_->SetRotY(action_->GetRotY());
				drone_->SetIsDroneSurvival(true);
			}
		}
		else
		{
			//アクションモード
			modeFlag_ = false;
			drone_->SetDrawFlag(true);
			IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
		}
		//UI表示切替
		ui_->UIChange(modeFlag_);
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
