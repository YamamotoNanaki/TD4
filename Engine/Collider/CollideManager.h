#pragma once
#include "Collider.h"
#include <list>
#include <d3d12.h>
#include "CollisionPrimitive.h"
#include "EditorMode.h"

namespace IFE
{
	class CollideManager
	{
		std::list<ColliderComponent*>colliders_;
	private:
		CollideManager() {};
		CollideManager(const CollideManager& obj) = delete;
		CollideManager& operator=(const CollideManager& obj) = delete;
	public:
		static CollideManager* Instance();
		void RaycastSystemUpdate();
		void CollidersUpdate();
		void Reset();
		void ColliderSet(ColliderComponent* collider);
		bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
		bool Raycast(const Ray& ray, uint16_t attribute, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	private:
		void PushBack(ColliderComponent* colA, ColliderComponent* colB, const Vector3& reject);
		void OnColliderHit(ColliderComponent* colA, ColliderComponent* colB);
	};
}
