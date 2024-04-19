#include "Player.h"
#include"Transform.h"
#include "Input.h"

void Player::Initialize()
{
	transform_->position_ = { 0,0,0 };
}

void Player::Update()
{
	transform_->position_ += IFE::Input::GetLXAnalog();
	transform_->position_ += IFE::Input::GetKeyPush(IFE::Key::Enter);
}

void Player::Draw()
{
}

void Player::Finalize()
{
}
