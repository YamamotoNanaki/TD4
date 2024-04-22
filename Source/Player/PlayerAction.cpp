#include "PlayerAction.h"
#include "Collider.h"
#include "Input.h"
#include"Transform.h"

void PlayerAction::Initialize()
{

}

void PlayerAction::Update()
{
}

void PlayerAction::Draw()
{
}

void PlayerAction::Finalize()
{
}

void PlayerAction::OnColliderHit(IFE::Collider collider)
{
}

void PlayerAction::Move()
{
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		transform_->position_ += { -1,0,0 };
	}
	if (IFE::Input::GetKeyPush(IFE::Key::RIGHT))
	{
		transform_->position_ += { 1, 0, 0 };
	}if (IFE::Input::GetKeyPush(IFE::Key::UP))
	{
		transform_->position_ += { 0, 0, 1 };
	}if (IFE::Input::GetKeyPush(IFE::Key::DOWN))
	{
		transform_->position_ += { 0, 0, -1 };
	}
}