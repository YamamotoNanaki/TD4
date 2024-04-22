#include "PlayerDrone.h"
#include "Collider.h"
#include"Object3D.h"
#include "Input.h"
#include"Transform.h"

void PlayerDrone::Initialize()
{
	objectPtr_->DrawFlag_ = false;
}

void PlayerDrone::Update()
{
}

void PlayerDrone::Draw()
{
}

void PlayerDrone::Finalize()
{
}

void PlayerDrone::OnColliderHit(IFE::Collider collider)
{
}

void PlayerDrone::Move()
{
	if (IFE::Input::GetKeyPush(IFE::Key::LEFT))
	{
		transform_->position_ += { -1, 0, 0 };
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

bool PlayerDrone::GetDrawFlag()
{
	return objectPtr_->DrawFlag_;
}

void PlayerDrone::SetDrawFlag(bool flag)
{
	objectPtr_->DrawFlag_ = flag;
}

void PlayerDrone::SetPos(const IFE::Vector3& pos)
{
	transform_->position_ = pos;
}