#include "PlayerCommonCamera.h"
#include "object3D.h"
#include "Transform.h"

using namespace IFE;

void PlayerCommonCamera::Initialize()
{
	col = cameraPtr_->GetComponent<Collider>();
}

void PlayerCommonCamera::Update()
{
	if (col)
	{
		auto c = col->GetCollider(0);
		Vector3 front = transformCamera_->target_ - transformCamera_->eye_;
		front.Normalize();
		c->rayDir_ = front;
	}
}

void PlayerCommonCamera::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	if (hitCollider->objectPtr_->GetObjectName().find("wall") != std::string::npos)
	{
		hitCollider->objectPtr_->DrawFlag_ = false;
	}
	myCollider;
}
