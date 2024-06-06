#include "CollideManager.h"
#include "CollisionPrimitive.h"
#include "Collision.h"
#include "Object3D.h"
#include "Transform.h"
#include "Emitter.h"

using namespace IFE;
using namespace std;

CollideManager* CollideManager::Instance()
{
	static CollideManager inst;
	return &inst;
}

void IFE::CollideManager::RaycastSystemUpdate()
{
	static const float adsDistance = 0.2f;
	for (auto itr : colliders_)
	{
		if (itr->GetGroundJudgeFlag())
		{
			Ray ray;
			ray.start = itr->GetColliderPosition();
			ray.start.y += itr->GetColliderScale().y;
			ray.dir = { 0,-1,0 };
			if (Raycast(ray, (uint16_t)Attribute::LANDSHAPE, &itr->groundHit_, itr->GetColliderScale().y * 2 + adsDistance))
			{
				itr->onGround_ = true;
				itr->transform_->position_.y -= (itr->groundHit_.distance - itr->GetColliderScale().y * 2.0f);
			}
			else
			{
				itr->onGround_ = false;
			}
			QuerySphere(itr, (uint16_t)Attribute::LANDSHAPE);
		}
	}
}

void IFE::CollideManager::CollidersUpdate()
{
	if (colliders_.size() < 2)return;
	list<ColliderCore*>::iterator itA;
	list<ColliderCore*>::iterator itB;

	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != colliders_.end(); ++itB)
		{
			ColliderCore* colA = *itA;
			ColliderCore* colB = *itB;
			if ((colA->objectPtr_ || colB->objectPtr_) && colA->objectPtr_ == colB->objectPtr_)continue;
			if ((colA->emitterPtr_ || colB->emitterPtr_) && colA->emitterPtr_ == colB->emitterPtr_)continue;
			if ((colA->cameraPtr_ || colB->cameraPtr_) && colA->cameraPtr_ == colB->cameraPtr_)continue;
			if (colA->attribute_ == uint16_t(Attribute::LANDSHAPE) == colB->attribute_)continue;

			//ともに球
			if (colA->GetColliderType() == ColliderType::SPHERE && colB->GetColliderType() == ColliderType::SPHERE)
			{
				Sphere SphereA(colA->GetColliderPosition(), Average(colA->GetColliderScale()));
				Sphere SphereB(colB->GetColliderPosition(), Average(colB->GetColliderScale()));
				Vector3 inter;
				Vector3 reject;
				if (Collision::CheckSphere(SphereA, SphereB, &inter, &reject))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					PushBack(colA, colB, reject);
					OnColliderHit(colA, colB);
				}
			}
			//メッシュと球
			else if (colA->GetColliderType() == ColliderType::MESH && colB->GetColliderType() == ColliderType::SPHERE)
			{
				MeshCollider* mesh = colA->GetMeshCollider();
				Sphere sphere(colB->GetColliderPosition(), Average(colB->GetColliderScale()));
				Vector3 inter;
				Vector3 reject;
				if (mesh->CheckCollisionSphere(sphere, &inter, &reject))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					PushBack(colA, colB, reject);
					OnColliderHit(colA, colB);
				}
			}
			//メッシュと球
			else if (colA->GetColliderType() == ColliderType::SPHERE && colB->GetColliderType() == ColliderType::MESH)
			{
				MeshCollider* mesh = colB->GetMeshCollider();
				Sphere sphere(colA->GetColliderPosition(), Average(colA->GetColliderScale()));
				Vector3 inter;
				Vector3 reject;
				if (mesh->CheckCollisionSphere(sphere, &inter, &reject))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					PushBack(colA, colB, reject);
					OnColliderHit(colA, colB);
				}
			}
			//レイと球
			else if (colA->GetColliderType() == ColliderType::RAY && colB->GetColliderType() == ColliderType::SPHERE)
			{
				Ray ray(colA->GetColliderPosition(), colA->rayDir_);
				Sphere sphere(colB->GetColliderPosition(), Average(colB->GetColliderScale()));
				Vector3 inter;
				float dis;
				float* hitdis = nullptr;
				if (colA->rayHittingdistance > 0.f)
				{
					hitdis = &colA->rayHittingdistance;
				}
				if (Collision::CheckRaySphere(ray, sphere, &dis, hitdis, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->rayDistance = dis;
					colB->rayDistance = dis;
					OnColliderHit(colA, colB);
				}
			}
			//レイとメッシュ
			else if (colA->GetColliderType() == ColliderType::RAY && colB->GetColliderType() == ColliderType::MESH)
			{
				MeshCollider* mesh = colB->GetMeshCollider();
				Ray ray(colA->GetColliderPosition(), colA->rayDir_);
				Vector3 inter;
				float dis;
				float* hitdis = nullptr;
				if (colA->rayHittingdistance > 0.f)
				{
					hitdis = &colA->rayHittingdistance;
				}
				if (mesh->CheckCollisionRay(ray, &dis, hitdis, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->rayDistance = dis;
					colB->rayDistance = dis;
					OnColliderHit(colA, colB);
				}
			}
			//レイと球
			else if (colA->GetColliderType() == ColliderType::SPHERE && colB->GetColliderType() == ColliderType::RAY)
			{
				Ray ray(colB->GetColliderPosition(), colB->rayDir_);
				Sphere sphere(colA->GetColliderPosition(), Average(colA->GetColliderScale()));
				Vector3 inter;
				float dis = 0;
				float* hitdis = nullptr;
				if (colB->rayHittingdistance > 0.f)
				{
					hitdis = &colB->rayHittingdistance;
				}
				if (Collision::CheckRaySphere(ray, sphere, &dis, hitdis, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->rayDistance = dis;
					colB->rayDistance = dis;
					OnColliderHit(colA, colB);
				}
			}
			//レイとメッシュ
			else if (colA->GetColliderType() == ColliderType::MESH && colB->GetColliderType() == ColliderType::RAY)
			{
				MeshCollider* mesh = colA->GetMeshCollider();
				Ray ray(colB->GetColliderPosition(), colB->rayDir_);
				Vector3 inter;
				float dis;
				float* hitdis = nullptr;
				if (colB->rayHittingdistance > 0.f)
				{
					hitdis = &colB->rayHittingdistance;
				}
				if (mesh->CheckCollisionRay(ray, &dis, hitdis, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->rayDistance = dis;
					colB->rayDistance = dis;
					OnColliderHit(colA, colB);
				}
			}
			//ともにOBB
			if (colA->GetColliderType() == ColliderType::OBB && colB->GetColliderType() == ColliderType::OBB)
			{
				OBB OBBA(colA->GetColliderPosition(), colA->transform_->matRot_, colA->GetColliderScale());
				OBB OBBB(colB->GetColliderPosition(), colB->transform_->matRot_, colB->GetColliderScale());
				Vector3 inter;
				Vector3 reject;
				if (Collision::CheckOBB(OBBA, OBBB, &inter, &reject))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					PushBack(colA, colB, reject);
					OnColliderHit(colA, colB);
				}
			}
			//OBBと球
			if (colA->GetColliderType() == ColliderType::SPHERE && colB->GetColliderType() == ColliderType::OBB)
			{
				Sphere sphere(colA->GetColliderPosition(), Average(colA->GetColliderScale()));
				OBB OBB(colB->GetColliderPosition(), colB->transform_->matRot_, colB->GetColliderScale());
				Vector3 inter;
				Vector3 reject;
				if (Collision::CheckOBBSphere(OBB, sphere, &inter, &reject))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					PushBack(colA, colB, reject);
					OnColliderHit(colA, colB);
				}
			}
			//OBBと球
			if (colA->GetColliderType() == ColliderType::OBB && colB->GetColliderType() == ColliderType::SPHERE)
			{
				OBB OBB(colA->GetColliderPosition(), colA->transform_->matRot_, colA->GetColliderScale());
				Sphere sphere(colB->GetColliderPosition(), Average(colB->GetColliderScale()));
				Vector3 inter;
				Vector3 reject;
				if (Collision::CheckOBBSphere(OBB, sphere, &inter, &reject))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					PushBack(colA, colB, reject);
					OnColliderHit(colA, colB);
				}
			}
			//OBBとレイ
			if (colA->GetColliderType() == ColliderType::OBB && colB->GetColliderType() == ColliderType::RAY)
			{
				OBB OBB(colA->GetColliderPosition(), colA->transform_->matRot_, colA->GetColliderScale());
				Ray ray(colB->GetColliderPosition(), colB->rayDir_);
				Vector3 inter;
				float dis;
				float* hitdis = nullptr;
				if (colB->rayHittingdistance > 0.f)
				{
					hitdis = &colB->rayHittingdistance;
				}
				if (Collision::CheckOBBRay(OBB, ray, &dis, hitdis, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->rayDistance = dis;
					colB->rayDistance = dis;
					OnColliderHit(colA, colB);
				}
			}
			//OBBとレイ
			if (colA->GetColliderType() == ColliderType::RAY && colB->GetColliderType() == ColliderType::OBB)
			{
				OBB OBB(colB->GetColliderPosition(), colB->transform_->matRot_, colB->GetColliderScale());
				Ray ray(colA->GetColliderPosition(), colA->rayDir_);
				Vector3 inter;
				float dis;
				float* hitdis = nullptr;
				if (colA->rayHittingdistance > 0.f)
				{
					hitdis = &colA->rayHittingdistance;
				}
				if (Collision::CheckOBBRay(OBB, ray, &dis, hitdis, &inter))
				{
					colA->interPoint_ = inter;
					colB->interPoint_ = inter;
					colA->rayDistance = dis;
					colB->rayDistance = dis;
					OnColliderHit(colA, colB);
				}
			}
		}
	}
	RaycastSystemUpdate();
}

void IFE::CollideManager::Reset()
{
	colliders_.clear();
}

void IFE::CollideManager::ColliderSet(ColliderCore* collider)
{
	colliders_.push_back(collider);
}

bool IFE::CollideManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	return Raycast(ray, (uint16_t)Attribute::ALL, hitInfo, maxDistance);
}

bool IFE::CollideManager::Raycast(const Ray& ray, uint16_t attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	std::list<ColliderCore*>::iterator it;
	std::list<ColliderCore*>::iterator it_hit;
	float distance = maxDistance;
	Vector3 inter;

	// 全てのコライダーと総当りチェック
	it = colliders_.begin();
	for (; it != colliders_.end(); ++it) {
		ColliderCore* colA = *it;

		// 属性が合わなければスキップ
		if (!(colA->attribute_ & attribute)) {
			continue;
		}

		if (colA->GetColliderType() == ColliderType::SPHERE) {
			Sphere sphere(colA->GetColliderPosition(), Average(colA->GetColliderScale()));
			float tempDistance;
			Vector3 tempInter;

			if (!Collision::CheckRaySphere(ray, sphere, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		else if (colA->GetColliderType() == ColliderType::OBB) {
			OBB obb(colA->GetColliderPosition(), colA->transform_->matRot_, colA->GetColliderScale());
			float tempDistance;
			Vector3 tempInter;

			if (!Collision::CheckOBBRay(obb, ray, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		else if (colA->GetColliderType() == ColliderType::MESH) {
			MeshCollider* meshCollider = colA->GetMeshCollider();

			float tempDistance;
			Vector3 tempInter;
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
	}

	return result;
}

void IFE::CollideManager::QuerySphere(ColliderCore* c, uint16_t attribute)
{
	std::list<ColliderCore*>::iterator it;
	Sphere sphere(c->GetColliderPosition(), Average(c->GetColliderScale()));

	// 全てのコライダーと総当りチェック
	it = colliders_.begin();
	for (; it != colliders_.end(); ++it) {
		ColliderCore* col = *it;

		// 属性が合わなければスキップ
		if (!(col->attribute_ & attribute)) {
			continue;
		}

		// 球
		if (col->GetColliderType() == ColliderType::SPHERE) {
			Sphere sphereB(col->GetColliderPosition(), Average(col->GetColliderScale()));

			Vector3 tempInter;
			Vector3 tempReject;
			if (!Collision::CheckSphere(sphere, sphereB, &tempInter, &tempReject)) continue;
			QueryPushBack(c, tempReject);
		}
		// メッシュ
		else if (col->GetColliderType() == ColliderType::MESH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			Vector3 tempInter;
			Vector3 tempReject;
			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;
			QueryPushBack(c, tempReject);
		}
		else if (col->GetColliderType() == ColliderType::OBB) {
			OBB obb(col->GetColliderPosition(), col->transform_->matRot_, col->GetColliderScale());

			Vector3 tempInter;
			Vector3 tempReject;
			if (!Collision::CheckOBBSphere(obb, sphere, &tempInter, &tempReject)) continue;
			QueryPushBack(c, tempReject);
		}
	}
}

void IFE::CollideManager::PushBack(ColliderCore* colA, ColliderCore* colB, const Vector3& reject)
{
	// 地面判定しきい値
	static const float threshold = cosf(ConvertToRadians(30.0f));
	static const Vector3 up = { 0,1,0 };
	if (colA->GetNoPushBackFlag() || colB->GetNoPushBackFlag())
	{
		return;
	}
	if (colA->GetPushBackFlag() && colB->GetPushBackFlag())
	{
		Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colA->transform_->MovePushBack(reject);
			colB->transform_->MovePushBack(-reject);
		}
	}
	else if (colA->GetPushBackFlag())
	{
		Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colA->transform_->MovePushBack(reject * 2);
		}
	}
	else if (colB->GetPushBackFlag())
	{
		Vector3 rejectDir = Vector3Normalize(reject);
		float cos = Vector3Dot(rejectDir, up);
		if (-threshold < cos && cos < threshold)
		{
			colB->transform_->MovePushBack(-reject * 2);
		}
	}
}

void IFE::CollideManager::OnColliderHit(ColliderCore* colA, ColliderCore* colB)
{
	if (colA->objectPtr_)
	{
		colA->objectPtr_->OnColliderHit(colA, colB);
	}
	else if (colA->emitterPtr_)
	{
		colA->emitterPtr_->OnColliderHit(colA, colB);
	}
	else if (colA->cameraPtr_)
	{
		colA->cameraPtr_->OnColliderHit(colA, colB);
	}
	if (colB->objectPtr_)
	{
		colB->objectPtr_->OnColliderHit(colB, colA);
	}
	else if (colB->emitterPtr_)
	{
		colB->emitterPtr_->OnColliderHit(colB, colA);
	}
	else if (colB->cameraPtr_)
	{
		colB->cameraPtr_->OnColliderHit(colB, colA);
	}
}

void IFE::CollideManager::QueryPushBack(ColliderCore* col, Vector3 reject)
{
	const Vector3 up = { 0,1,0 };

	Vector3 rejectDir = Vector3Normalize(reject);
	float cos = Vector3Dot(rejectDir, up);

	// 地面判定しきい値
	const float threshold = cosf(ConvertToRadians(30.0f));

	if (-threshold < cos && cos < threshold) {
		col->transform_->MovePushBack(reject);
	}
}
