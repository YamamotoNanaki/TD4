#include "BaseEnemy.h"
#include "Collider.h"
#include "Object3d.h"
#include "transform.h"
#include "ObjectManager.h"
#include "PlayerDrone.h"

using namespace IFE;

void IFE::BaseEnemy::Highlighting()
{
	Vector3 dir = ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->transform_->position_ - transform_->position_;
	dir.Normalize();
	objectPtr_->GetComponent<Collider>()->GetCollider(0)->rayDir_ = dir;

	droneHit_ = false;
	droneHitDistance_ = 0;
	if (hitObject_)
	{
		if (hitObject_->GetComponent<PlayerDrone>())
		{
			droneHit_ = true;
			droneHitDistance_ = hitDistance_;
		}
	}

	hitObject_ = nullptr;
	hitDistance_ = 0;
}

void IFE::BaseEnemy::Update()
{
	EnemyUpdate();
	Highlighting();
}

void IFE::BaseEnemy::OnColliderHit(ColliderCore* mycol, ColliderCore* hitcol)
{
	if (mycol->GetColliderType() == ColliderType::RAY)
	{
		if (mycol->rayDistance > -2)
		{
			if (hitcol->objectPtr_)
			{
				if (!hitObject_)
				{
					hitObject_ = hitcol->objectPtr_;
					hitDistance_ = mycol->rayDistance;
				}
				else
				{
					if (hitDistance_ > mycol->rayDistance)
					{
						hitObject_ = hitcol->objectPtr_;
						hitDistance_ = mycol->rayDistance;
					}
				}
			}
		}
	}
	EnemyOnColliderHit(mycol, hitcol);
}
