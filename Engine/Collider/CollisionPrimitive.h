#pragma once
#include "IFEMath.h"
#include <vector>
#include "EditorMode.h"

namespace IFE
{
	struct Sphere
	{
		Vector3 center{};
		float radius = 1.0f;
		Sphere(Vector3 center, float radius = 1.0f);
	};
	struct Plane
	{
		Vector3 normal = { 0,1,0 };
		float distance = 0.0f;
	};
	struct Triangle
	{
		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
		Vector3 normal;
		void ComputeNormal();
	};
	struct Ray
	{
		Vector3 start{};
		Vector3 dir = { 1,0,0 };
		Ray(Vector3 start, Vector3 dir);
		Ray() {};
	};
	class FBXModel;
	struct MeshCollider
	{
		std::vector<Triangle>triangles;
		Matrix* matWorld_;
		Matrix invMatWorld;
		MeshCollider(FBXModel* model, Matrix* matWorld);
		bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);
		bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);
		bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, float* hittingdistance = nullptr, Vector3* inter = nullptr);
		~MeshCollider() {};
	};
	class ColliderCore;
	struct RaycastHit
	{
		//衝突したコライダー
		ColliderCore* collider = nullptr;
		// 衝突点
		Vector3 inter;
		// 衝突点までの距離
		float distance = 0.0f;
	};
	struct OBB {
	public:
		Vector3 center;  // 中心点
		Vector3 axis[3]; // 局所軸
		Vector3 extents; // 各軸に沿った半径（長さ）

		OBB(Vector3 c, Vector3 a[3], Vector3 e)
			: center(c), extents(e) {
			axis[0] = a[0];
			axis[1] = a[1];
			axis[2] = a[2];
		}
		OBB(Vector3 c, const Quaternion& q, Vector3 e)
			: center(c), extents(e) {
			Vector3 baseX(1, 0, 0);
			Vector3 baseY(0, 1, 0);
			Vector3 baseZ(0, 0, 1);
			axis[0] = Rotate(q, baseX);
			axis[1] = Rotate(q, baseY);
			axis[2] = Rotate(q, baseZ);
		}
		OBB(Vector3 c, Matrix r, Vector3 e)
			: center(c), extents(e) {
			Vector3 baseX(1, 0, 0);
			Vector3 baseY(0, 1, 0);
			Vector3 baseZ(0, 0, 1);

			Vector3 rotatedX = Vector3Transform(baseX, r);
			Vector3 rotatedY = Vector3Transform(baseY, r);
			Vector3 rotatedZ = Vector3Transform(baseZ, r);

			axis[0] = rotatedX;
			axis[1] = rotatedY;
			axis[2] = rotatedZ;
		}
	};
	struct Cone {
		Float3 apex; // 頂点
		float radius; // 底面半径
		float height; // 高さ
		Cone();
		Cone(Float3 apex, float radius, float height);
	};
}

