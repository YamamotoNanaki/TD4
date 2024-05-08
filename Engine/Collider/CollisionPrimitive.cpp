#include "CollisionPrimitive.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "FBXModel.h"
#include "Collision.h"

void IFE::Triangle::ComputeNormal()
{
	Vector3 p0_p1 = p1 - p0;
	Vector3 p0_p2 = p2 - p0;

	normal = Vector3Cross(p0_p1, p0_p2);
	normal = Vector3Normalize(normal);
}

IFE::Sphere::Sphere(Vector3 center, float radius) :center(center), radius(radius) {}

IFE::MeshCollider::MeshCollider(FBXModel* model, Matrix* matWorld)
{
	matWorld_ = matWorld;
	invMatWorld = MatrixInverse(*matWorld);
	triangles.clear();
	triangles = model->GetMeshColliderTriangle();
}

bool IFE::MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3* inter, Vector3* reject)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Sphere localSphere(Vector3Transform(sphere.center, invMatWorld), Vector3Length({ invMatWorld.m[0][0],invMatWorld.m[0][1],invMatWorld.m[0][2] }));

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle& triangle = *it;

		if (Collision::CheckSphereTriangle(localSphere, triangle, inter, reject)) {
			if (inter) {
				*inter = Vector3Transform(*inter, *matWorld_);
			}
			if (reject) {
				*reject = Vector3TransformNormal(*reject, *matWorld_);
			}
			return true;
		}
	}

	return false;
}

bool IFE::MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, Vector3* inter)
{
	invMatWorld = MatrixInverse(*matWorld_);
	Ray localRay(Vector3Transform(ray.start, invMatWorld), Vector3TransformNormal(ray.dir, invMatWorld));

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	int i = 0;
	for (; it != triangles.cend(); ++it)
	{
		if (i == 5 || i == 9) {
			static int a = 0;
			a++;
		}
		i++;

		Vector3 tempInter;
		const Triangle& triangle = *it;
		if (Collision::CheckRayTriangle(localRay, triangle, nullptr, &tempInter))
		{
			tempInter = Vector3Transform(tempInter, *matWorld_);

			if (distance)
			{
				Vector3 sub = tempInter - ray.start;
				*distance = Vector3Dot(sub, ray.dir);
			}

			if (inter)
			{
				*inter = tempInter;
			}

			return true;
		}
	}
	return false;
}

IFE::Ray::Ray(Vector3 start, Vector3 dir) :start(start), dir(dir) {}
