#include "Player.h"
#include"Transform.h"
#include "Input.h"
#include "Collider.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include"ObjectManager.h"

void Player::Initialize()
{
	action_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
	drone_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>();

	transform_->position_ = { 0,0,0 };
}

void Player::Update()
{
	ChangeMode();

	if (modeFlag_ == false)
	{
		action_->Move();
	}
	else
	{

	}
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