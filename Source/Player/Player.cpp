#include "Player.h"
#include"Transform.h"
#include "Input.h"
#include "Collider.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include"ObjectManager.h"
#include"CameraManager.h"
#include"SpriteManager.h"

void Player::Initialize()
{
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	drone_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>();

	ChangeUI();

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
			modeFlag_ = true;
			drone_->SetDrawFlag(false);
			if (isDrone_ == false)
			{
				drone_->SetPos(action_->GetPos());
				isDrone_ = true;
			}
		}
		else
		{
			modeFlag_ = false;
			drone_->SetDrawFlag(true);
		}
		ChangeUI();
	}
}

void Player::ChangeUI()
{
	if (modeFlag_ == false)
	{
		IFE::CameraManager::Instance()->SetActiveCamera("DroneCamera");
		IFE::SpriteManager::Instance()->GetSpritePtr("flame")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Down")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Up")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeDrone")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("SpeedChange")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("RT")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("LT")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("YDrone")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = false;
	}
	else
	{
		IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
		IFE::SpriteManager::Instance()->GetSpritePtr("flame")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Down")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("Up")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeDrone")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("SpeedChange")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("RT")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("LT")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("YDrone")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = true;
		IFE::SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = true;
	}
}
