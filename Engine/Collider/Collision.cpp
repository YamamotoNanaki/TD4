#include "Collision.h"
#include <cmath>

using namespace IFE;

bool IFE::Collision::CheckSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	float distV = Vector3Dot(sphere.center, plane.normal);
	float dist = distV - plane.distance;
	if (fabsf(dist) > sphere.radius)return false;

	if (inter)*inter = -dist * plane.normal + sphere.center;

	return true;
}

bool IFE::Collision::CheckRaySphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	return CheckRaySphere(ray, sphere, distance, nullptr, inter);
}

bool IFE::Collision::CheckRaySphere(const Ray& ray, const Sphere& sphere, float* distance, float* rayHittingdistance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.center;
	float b = Vector3Dot(m, ray.dir);
	float c = Vector3Dot(m, m) - sphere.radius * sphere.radius;

	if (c > 0.0f && b > 0.0f)return false;
	float discr = b * b - c;
	if (discr < 0.0f)return false;

	float t = -b - sqrtf(discr);
	if (t < 0.0f)t = 0.0f;

	if (distance)
	{
		if (rayHittingdistance && rayHittingdistance > distance)
		{
			*distance = t;
			if (inter)
			{
				inter->x = ray.start.x + t * ray.dir.x;
				inter->y = ray.start.y + t * ray.dir.y;
				inter->z = ray.start.z + t * ray.dir.z;
			}
			return true;
		}
		else if (!rayHittingdistance)
		{
			*distance = t;
			return true;
		}
	}
	return false;
}

bool IFE::Collision::CheckRayPlane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	const float epsilon = 1.0e-5f;
	float d1 = Vector3Dot(plane.normal, ray.dir);

	if (d1 > -epsilon)return false;

	float d2 = Vector3Dot(plane.normal, ray.start);
	float dist = d2 - plane.distance;
	float t = dist / -d1;
	if (t < 0)return false;

	if (distance)*distance = t;
	if (inter)*inter = ray.start + t * ray.dir;
	return true;
}

bool IFE::Collision::CheckRayTriangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal;
	plane.distance = Vector3Dot(triangle.normal, triangle.p0);

	if (!CheckRayPlane(ray, plane, distance, &interPlane))return false;

	const float epsilon = 1.0e-5f;

	Vector3 m;
	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	m = Vector3Cross(pt_p0, p0_p1);

	if (Vector3Dot(m, triangle.normal) < -epsilon)return false;

	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 p1_p2 = triangle.p2 - triangle.p1;
	m = Vector3Cross(pt_p1, p1_p2);

	if (Vector3Dot(m, triangle.normal) < -epsilon)return false;

	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 p2_p0 = triangle.p0 - triangle.p2;
	m = Vector3Cross(pt_p2, p2_p0);

	if (Vector3Dot(m, triangle.normal) < -epsilon)return false;

	if (inter)*inter = interPlane;

	return true;
}

bool IFE::Collision::CheckSphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter, Vector3* reject)
{
	// ���S�_�̋�����2�� <= ���a�̘a��2��@�Ȃ����
	float dist = Vector3Dot(sphereA.center - sphereB.center, sphereA.center - sphereB.center);

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2) {
		if (inter) {
			// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = VectorLerp(sphereA.center, sphereB.center, t);
		}
		// �����o���x�N�g�����v�Z
		if (reject) {
			float rejectLen = sphereA.radius + sphereB.radius - sqrtf(dist);
			*reject = Vector3Normalize(sphereA.center - sphereB.center);
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool IFE::Collision::CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter, Vector3* reject)
{
	Vector3 p;
	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// �_p�Ƌ��̒��S�̍����x�N�g��
	Vector3 v = p - sphere.center;
	// �����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	float distanceSquare = Vector3Dot(v, v);
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (distanceSquare > sphere.radius * sphere.radius)	return false;
	// �[����_���v�Z
	if (inter) {
		// �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}
	// �����o���x�N�g�����v�Z
	if (reject) {
		float ds = Vector3Dot(sphere.center, triangle.normal);
		float dt = Vector3Dot(triangle.p0, triangle.normal);
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}
	return true;
}

bool IFE::Collision::CheckOBB(const OBB& box1, const OBB& box2, Vector3* inter, Vector3* reject)
{
	float minOverlap = std::numeric_limits<float>::max();
	Vector3 smallestAxis;
	bool isSeparated = false; // �������������ꂽ���ǂ���

	// �e���ŕ������藝��K�p
	for (int i = 0; i < 3; i++) {
		Vector3 axis = box1.axis[i];
		if (IsSeparatedByAxis(axis, box1, box2)) {
			isSeparated = true;
			break;
		}
	}
	for (int i = 0; i < 3; i++) {
		Vector3 axis = box2.axis[i];
		if (IsSeparatedByAxis(axis, box1, box2)) {
			isSeparated = true;
			break;
		}
	}

	if (!isSeparated) {
		if (inter)*inter = smallestAxis * minOverlap; // �����o���x�N�g���̌v�Z
		if (reject)*reject = (box1.center + box2.center) * 0.5f; // �Փ˒n�_�̊ȈՌv�Z
		return true;
	}
	return false;
}

bool IFE::Collision::CheckOBBSphere(const OBB& box, const Sphere& sphere, Vector3* inter, Vector3* reject)
{
	return CheckOBBSphere(box, sphere, inter, reject, nullptr);
}

bool IFE::Collision::CheckOBBSphere(const OBB& box, const Sphere& sphere, Vector3* inter, Vector3* reject, float* dis)
{
	// OBB�̒��S���狅�̒��S�܂ł̃x�N�g��
	Vector3 d = sphere.center - box.center;

	// ���̒��S��OBB�̋Ǐ����W�n�ɓ��e����
	Vector3 closestPoint = box.center;
	float dist;
	Vector3 displacement;

	// �e���ɑ΂��鏈��
	float extents[] = { box.extents.x, box.extents.y, box.extents.z };

	for (int i = 0; i < 3; i++) {
		// OBB�̋Ǐ����ɉ����Ē��S����̋������v�Z
		dist = d.Dot(box.axis[i]);
		if (dist > extents[i]) dist = extents[i];
		if (dist < -extents[i]) dist = -extents[i];
		closestPoint += box.axis[i] * dist;
	}

	displacement = closestPoint - sphere.center;
	float distanceSquared = displacement.LengthSquared();
	float radiusSquared = sphere.radius * sphere.radius;

	// ���̒��S�ƍł��߂��_�Ƃ̋��������̔��a�ȉ��Ȃ�Փ�
	if (distanceSquared < radiusSquared) {
		float distance = sqrt(distanceSquared);
		if (inter)*inter = closestPoint; // �Փ˒n�_�͍ł��߂��_
		if (reject)
		{
			if (distance == 0) {
				// �Փ˓_�����̒��S�̏ꍇ�i�����邽�߂ɔC�ӂ̕����ɏ����ړ��j
				*reject = box.axis[0] * sphere.radius;
			}
			else {
				*reject = -displacement * ((sphere.radius - distance) / distance); // �����o���x�N�g��
			}
		}
		if (dis)*dis = distance;
		return true;
	}

	return false;
}

bool IFE::Collision::CheckOBBTriangle(const OBB& obb, const Triangle& triangle, Vector3* inter, Vector3* reject)
{
	float minPoint[3] = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	float maxPoint[3] = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

	Vector3 vertices[3] = { triangle.p0,triangle.p1,triangle.p2, };
	// �e���ɑ΂��鏈��
	float extents[] = { obb.extents.x, obb.extents.y, obb.extents.z };

	Vector3 penetrationDepths[3]; // �e���ł̐Z���[�x���L�^

	// �O�p�`�̒��_��OBB�̋Ǐ����W�n�ɕϊ����܂�
	for (int i = 0; i < 3; ++i) {
		Vector3 localVertex = vertices[i] - obb.center;
		for (int j = 0; j < 3; ++j) {
			float projection = localVertex.Dot(obb.axis[j]);
			minPoint[j] = std::min(minPoint[j], projection);
			maxPoint[j] = std::max(maxPoint[j], projection);
		}
	}

	// �O�p�`��OBB�̊O���ɂ��邩�ǂ������`�F�b�N���܂�
	Vector3 pushDirection;
	float minPenetrationDepth = std::numeric_limits<float>::max();
	for (int i = 0; i < 3; ++i) {
		float penetrationDepth = 0;
		if (maxPoint[i] < -extents[i] || minPoint[i] > extents[i]) {
			return false; // �Փ˂��Ă��܂���
		}
		// �����o���x�N�g���̌v�Z
		float minDepth = extents[i] - minPoint[i]; // min���̐Z���[�x
		float maxDepth = extents[i] + maxPoint[i]; // max���̐Z���[�x
		if (minDepth < maxDepth) {
			penetrationDepth = minDepth;
			pushDirection = obb.axis[i] * -1; // �������ɏC��
		}
		else {
			penetrationDepth = maxDepth;
			pushDirection = obb.axis[i]; // �O����
		}

		if (penetrationDepth < minPenetrationDepth) {
			minPenetrationDepth = penetrationDepth;
			if (reject)*reject = pushDirection * penetrationDepth;
		}
	}

	// �Փ˒n�_�̌v�Z�i�����ł͊ȗ�������Ă��܂��j
	if (inter && reject)*inter = obb.center + (*reject) * 0.5; // ����͒P�Ȃ鐄��ł�
	if (inter)*inter = obb.center; // ����͒P�Ȃ鐄��ł�

	return true; // �Փ˂����o����܂���
}

bool IFE::Collision::CheckOBBRay(const OBB& box, const Ray& ray, float* distance, float* rayHittingdistance, Vector3* inter)
{
	return CheckOBBRay(box, ray, distance, nullptr, rayHittingdistance, inter);
}

bool IFE::Collision::CheckOBBRay(const OBB& box, const Ray& ray, float* distance, float* distanceFar, float* rayHittingdistance, Vector3* inter)
{
	float tMin = 0.0f; // ���C��OBB�ɓ���ŏ���t�l
	float tMax = std::numeric_limits<float>::max(); // ���C��OBB����o��ő��t�l

	Vector3 delta = box.center - ray.start; // ���C�̌��_����OBB�̒��S�ւ̃x�N�g��

	// �e���ɑ΂��鏈��
	float extents[] = { box.extents.x, box.extents.y, box.extents.z };

	// �e���Ō����`�F�b�N
	for (int i = 0; i < 3; i++) {
		Vector3 axis = box.axis[i];
		float e = axis.Dot(delta); // OBB���S�ւ̃x�N�g�������ɓ��e
		float f = ray.dir.Dot(axis); // ���C���������ɓ��e

		if (fabs(f) > 0.001f) { // ���C���������ɑ΂��Ăقڕ��s�łȂ��ꍇ
			float t1 = (e + extents[i]) / f;
			float t2 = (e - extents[i]) / f;

			if (t1 > t2) {
				std::swap(t1, t2);
			}

			tMin = std::max(tMin, t1);
			tMax = std::min(tMax, t2);

			if (tMin > tMax) return false; // �������m�F���ꂽ�ꍇ
		}
		else { // ���C�����ɑ΂��ĕ��s�ŁA�͈͊O�ɂ���ꍇ
			if (-e + extents[i] < 0 || -e - extents[i] > 0)
				return false;
		}
	}
	if (rayHittingdistance)
	{
		// �������邪�A�w�肳�ꂽ�����ȊO�ɂ���ꍇ
		if (tMin > *rayHittingdistance) return false;

		// �L���Ȍ����_������ɂ���ꍇ
		if (tMin < 0 && tMax < 0) return false;

		float tHit = tMin >= 0 ? tMin : tMax; // ���ۂ̏Փ�t�l

		if (tHit <= *rayHittingdistance) {
			*inter = ray.start + ray.dir * tHit; // �Փ˒n�_
			*distance = tHit; // �Փ˂܂ł̋���
			if (distanceFar)
			{
				*distanceFar = tMax;
			}
			return true;
		}
	}
	else
	{
		// �L���Ȍ����_������ɂ���ꍇ
		if (tMin < 0 && tMax < 0) return false;

		float tHit = tMin >= 0 ? tMin : tMax; // ���ۂ̏Փ�t�l

		*inter = ray.start + ray.dir * tHit; // �Փ˒n�_
		*distance = tHit; // �Փ˂܂ł̋���
		if (distanceFar)
		{
			*distanceFar = tMax;
		}
		return true;
	}

	return false;
}

bool IFE::Collision::CheckOBBRay(const OBB& box, const Ray& ray, float* distance, Vector3* inter)
{
	return CheckOBBRay(box, ray, distance, nullptr, inter);
}

//bool IF::Collision::CheckSphere(const Sphere& s1, const Sphere& s2, Vector3* inter)
//{
//	Vector3 vec = s2.center - s1.center;
//
//	float sqLength = vec.Length();
//	sqLength *= sqLength;
//
//	float r = s1.radius + s2.radius;
//
//	if (sqLength > r * r)return false;
//
//	if (inter)*inter = vec / 2;
//
//	return true;
//}

bool IFE::Collision::CheckConeSphere(const Cone& cone, const Sphere& sphere)
{
	// ���_���狅�̒��S�ւ̃x�N�g��
	Float3 apexToCenter = {
		sphere.center.x - cone.apex.x,
		sphere.center.y - cone.apex.y,
		sphere.center.z - cone.apex.z
	};

	// �~���̍��������x�N�g��
	Float3 coneDir = { 0.0f, 1.0f, 0.0f };

	// ���_���狅�̒��S�ւ̃x�N�g���ƍ��������x�N�g���̓��ς��v�Z
	float dotProduct = apexToCenter.x * coneDir.x + apexToCenter.y * coneDir.y + apexToCenter.z * coneDir.z;

	// ���e�_���~���̍������ɂ��邩�m�F
	if (dotProduct < 0.0f || dotProduct > cone.height) {
		return false;
	}

	// ���_���狅�̒��S�ւ̃x�N�g���̒����̓����v�Z
	float distanceSquared = apexToCenter.x * apexToCenter.x +
		apexToCenter.y * apexToCenter.y +
		apexToCenter.z * apexToCenter.z;

	// ���̒��S���~���̎�����ǂꂾ������Ă��邩�v�Z
	float radiusAtHeight = (dotProduct / cone.height) * cone.radius;
	float radiusSquared = radiusAtHeight * radiusAtHeight;
	float sphereRadiusSquared = sphere.radius * sphere.radius;

	return distanceSquared <= (radiusSquared + sphereRadiusSquared);
}

bool IFE::Collision::IsSeparatedByAxis(const Vector3 axis, const OBB& box1, const OBB& box2)
{
	auto projectBox = [](const OBB& box, const Vector3& axis) {
		float radius = box.extents.x * std::fabs(axis.Dot(box.axis[0])) +
			box.extents.y * std::fabs(axis.Dot(box.axis[1])) +
			box.extents.z * std::fabs(axis.Dot(box.axis[2]));
		float centerProjection = axis.Dot(box.center);
		return std::make_pair(centerProjection - radius, centerProjection + radius);
		};

	// �e�{�b�N�X�����ɉ����ē��e
	auto projection1 = projectBox(box1, axis);
	auto projection2 = projectBox(box2, axis);

	// �v���W�F�N�V�������d�Ȃ��Ă��Ȃ����m�F
	if (projection1.second < projection2.first || projection2.second < projection1.first) {
		return true; // �������m�F���ꂽ�ꍇ
	}

	return false; // �������m�F����Ȃ��ꍇ
}

void IFE::Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;

	float d1 = Vector3Dot(p0_p1, p0_pt);
	float d2 = Vector3Dot(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p1_pt = point - triangle.p1;

	float d3 = Vector3Dot(p0_p1, p1_pt);
	float d4 = Vector3Dot(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p2_pt = point - triangle.p2;

	float d5 = Vector3Dot(p0_p1, p2_pt);
	float d6 = Vector3Dot(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool IFE::Collision::BoxCollision2D(IFE::Float2 pos1, float r1, IFE::Float2 pos2, float r2)
{
	return (pos1.x - r1) < (pos2.x + r2) && (pos1.x + r1) > (pos2.x - r2) && (pos1.y - r1) < (pos2.y + r2) && (pos1.y + r1) > (pos2.y - r2);
}

//bool IF::Collision::CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
//{
//	Vector3 p;
//	ClosetPtPoint2Triangle(sphere.center, triangle, &p);
//	Vector3 v = p - sphere.center;
//
//	float d = Vector3Dot(v, v);
//	if (d > sphere.radius * sphere.radius)return false;
//	if (inter)*inter = p;
//	return true;
//}

//bool IF::Collision::CheckAABB(const Primitive& obj1, const Primitive& obj2)
//{
//	return ((obj1.v2.x > obj2.v1.x) &&
//		(obj1.v1.x < obj2.v2.x) &&
//		(obj1.v2.y > obj2.v1.y) &&
//		(obj1.v1.y < obj2.v2.y) &&
//		(obj1.v2.z > obj2.v1.z) &&
//		(obj1.v1.z < obj2.v2.z));
//}

bool IFE::Collision::CheckCircle(const Circle& Circle1, const Circle& Circle2)
{
	return ((Circle2.pos.x - Circle1.pos.x) * (Circle2.pos.x - Circle1.pos.x) +
		(Circle2.pos.y - Circle1.pos.y) * (Circle2.pos.y - Circle1.pos.y) <=
		(Circle1.radisu + Circle2.radisu) * (Circle1.radisu + Circle2.radisu));
}

//bool IF::Collision::CheckCircleXYAABB(const Primitive& Circle, const Primitive& box)
//{
//	Primitive a;
//	a.SetMinPos({ Circle.v1.x - Circle.f,Circle.v1.y - Circle.f,Circle.v1.z - Circle.f });
//	a.SetMaxPos({ Circle.v1.x + Circle.f,Circle.v1.y + Circle.f,Circle.v1.z + Circle.f });
//	if (CheckAABB(box, a))
//	{
//		if ((Circle.v1.x > box.v1.x) && (Circle.v1.x < box.v2.x) &&
//			(Circle.v1.y > box.v2.y - Circle.f) && (Circle.v1.y < box.v1.y + Circle.f))return true;
//		if ((Circle.v1.x > box.v1.x - Circle.f) && (Circle.v1.x < box.v2.x + Circle.f) &&
//			(Circle.v1.y > box.v2.y) && (Circle.v1.y < box.v1.y))return true;
//		if ((box.v1.x - Circle.v1.x) * (box.v1.x - Circle.v1.x) + (box.v2.y - Circle.v1.y) *
//			(box.v2.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//		if ((box.v1.x - Circle.v1.x) * (box.v1.x - Circle.v1.x) + (box.v1.y - Circle.v1.y) *
//			(box.v1.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//		if ((box.v2.x - Circle.v1.x) * (box.v2.x - Circle.v1.x) + (box.v2.y - Circle.v1.y) *
//			(box.v2.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//		if ((box.v2.x - Circle.v1.x) * (box.v2.x - Circle.v1.x) + (box.v1.y - Circle.v1.y) *
//			(box.v1.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//	}
//	return false;
//}
