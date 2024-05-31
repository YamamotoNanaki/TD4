#include "Player.h"
#include"Transform.h"
#include "Input.h"
#include "Collider.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include"ObjectManager.h"
#include"CameraManager.h"

void Player::Initialize()
{
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	drone_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>();

	IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");

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
			IFE::CameraManager::Instance()->SetActiveCamera("DroneCamera");
			
		}
		else
		{
			modeFlag_ = false;
			drone_->SetDrawFlag(true);
			IFE::CameraManager::Instance()->SetActiveCamera("ActionCamera");
		}
	}
}