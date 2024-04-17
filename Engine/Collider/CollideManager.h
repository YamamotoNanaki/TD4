#pragma once
#include "Collider.h"
#include <list>
#include <d3d12.h>
#include "CollisionPrimitive.h"

namespace IFE
{
	class CollideManager
	{
		std::list<Collider*>colliders_;
	private:
		CollideManager() {};
		CollideManager(const CollideManager& obj) = delete;
		CollideManager& operator=(const CollideManager& obj) = delete;
	public:
		static CollideManager* Instance();
		void RaycastSystemUpdate();
		void CollidersUpdate();
		void Reset();
		void ColliderSet(Collider* collider);
		bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
		bool Raycast(const Ray& ray, uint16_t attribute, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	private:
		void PushBack(Collider* colA, Collider* colB, const Vector3& reject);
		void OnColliderHit(Collider* colA, Collider* colB);
	};
}
