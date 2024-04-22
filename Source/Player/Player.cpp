#include "Player.h"
#include"Transform.h"
#include "Input.h"
#include "Collider.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"

void Player::Initialize()
{
	transform_->position_ = { 0,0,0 };
}

void Player::Update()
{
	ChangeMode();
}

void Player::Draw()
{
}

void Player::Finalize()
{
}

void Player::OnColliderHit(IFE::Collider collider)
{
}

void Player::ChangeMode()
{
	if (IFE::Input::GetKeyPush(IFE::Key::Y))
	{
		if (modeFlag_ == false)
		{
			modeFlag_ = true;
		}
		else
		{
			modeFlag_ = false;
		}
	}
}