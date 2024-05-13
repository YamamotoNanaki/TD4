#pragma once
#include "CollisionPrimitive.h"
#include "EditorMode.h"

namespace IFE
{
	class Collision
	{
	public:
		static bool CheckSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
		static bool CheckRaySphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);
		static bool CheckRaySphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, float* rayHittingdistance = nullptr, Vector3* inter = nullptr);
		static bool CheckRayPlane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);
		static bool CheckRayTriangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);
		//static bool CheckSphere(const Sphere& s1, const Sphere& s2, Vector3* inter = nullptr);
		static bool CheckSphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr, Vector3* reject = nullptr);
		//static bool CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);
		static bool CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr, Vector3* reject = nullptr);
		static bool CheckOBB(const OBB& box1, const OBB& box2, Vector3* inter = nullptr, Vector3* reject = nullptr);
		static bool CheckOBBSphere(const OBB& box, const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);
		static bool CheckOBBTriangle(const OBB& box, const Triangle& triangle, Vector3* inter = nullptr, Vector3* reject = nullptr);
		static bool CheckOBBRay(const OBB& box, const Ray& ray, float* distance = nullptr, float* rayHittingdistance = nullptr, Vector3* inter = nullptr);
		//static bool CheckAABB(const Primitive& box1, const Primitive& box2);
		//static bool CheckCircleXY(const Primitive& Circle1, const Primitive& Circle2);
		//static bool CheckCircleXYAABB(const Primitive& Circle, const Primitive& box);
	private:
		static bool IsSeparatedByAxis(const Vector3 axis, const OBB& box1, const OBB& box2);
		static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

		//2d
	public:

		static bool BoxCollision2D(IFE::Float2 pos1, float r1, IFE::Float2 pos2, float r2);
	};
}
