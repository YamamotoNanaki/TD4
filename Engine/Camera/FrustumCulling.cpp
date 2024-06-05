#include "FrustumCulling.h"

using namespace IFE;

void IFE::FrustumCulling::Initialize(Matrix* viewMatrix, Matrix* ProjMatrix)
{
	viewMatrix_ = viewMatrix;
	ProjMatrix_ = ProjMatrix;
}

void IFE::FrustumCulling::Update()
{
	Matrix viewProjMatrix = *viewMatrix_ * *ProjMatrix_;

	planes[0].x = viewProjMatrix.m[0][3] + viewProjMatrix.m[0][0]; // Left
	planes[0].y = viewProjMatrix.m[1][3] + viewProjMatrix.m[1][0];
	planes[0].z = viewProjMatrix.m[2][3] + viewProjMatrix.m[2][0];
	planes[0].w = viewProjMatrix.m[3][3] + viewProjMatrix.m[3][0];

	planes[1].x = viewProjMatrix.m[0][3] - viewProjMatrix.m[0][0]; // Right
	planes[1].y = viewProjMatrix.m[1][3] - viewProjMatrix.m[1][0];
	planes[1].z = viewProjMatrix.m[2][3] - viewProjMatrix.m[2][0];
	planes[1].w = viewProjMatrix.m[3][3] - viewProjMatrix.m[3][0];

	planes[2].x = viewProjMatrix.m[0][3] + viewProjMatrix.m[0][1]; // Bottom
	planes[2].y = viewProjMatrix.m[1][3] + viewProjMatrix.m[1][1];
	planes[2].z = viewProjMatrix.m[2][3] + viewProjMatrix.m[2][1];
	planes[2].w = viewProjMatrix.m[3][3] + viewProjMatrix.m[3][1];

	planes[3].x = viewProjMatrix.m[0][3] - viewProjMatrix.m[0][1]; // Top
	planes[3].y = viewProjMatrix.m[1][3] - viewProjMatrix.m[1][1];
	planes[3].z = viewProjMatrix.m[2][3] - viewProjMatrix.m[2][1];
	planes[3].w = viewProjMatrix.m[3][3] - viewProjMatrix.m[3][1];

	planes[4].x = viewProjMatrix.m[0][2]; // Near
	planes[4].y = viewProjMatrix.m[1][2];
	planes[4].z = viewProjMatrix.m[2][2];
	planes[4].w = viewProjMatrix.m[3][2];

	planes[5].x = viewProjMatrix.m[0][3] - viewProjMatrix.m[0][2]; // Far
	planes[5].y = viewProjMatrix.m[1][3] - viewProjMatrix.m[1][2];
	planes[5].z = viewProjMatrix.m[2][3] - viewProjMatrix.m[2][2];
	planes[5].w = viewProjMatrix.m[3][3] - viewProjMatrix.m[3][2];


	// 平面を正規化する
	for (int i = 0; i < 6; i++) {
		float length = sqrtf(planes[i].x * planes[i].x + planes[i].y * planes[i].y + planes[i].z * planes[i].z);
		planes[i].x /= length;
		planes[i].y /= length;
		planes[i].z /= length;
		planes[i].w /= length;
	}
}

bool IFE::FrustumCulling::IsSphereInFrustum(Sphere spere)
{
	for (int i = 0; i < 6; i++) {
		float distance = planes[i].x * spere.center.x + planes[i].y * spere.center.y + planes[i].z * spere.center.z + planes[i].w;
		if (distance < -spere.radius) {
			return false; // 視錐台外にある
		}
	}
	return true; // 視錐台内にある
}

//bool IFE::FrustumCulling::IsOBBInFrustum(OBB obb)
//{
//    // OBBの頂点を計算
//    Vector3 vCenter = obb.center;
//    Vector3 vExtents = obb.extents;
//    Vector3 vAxis[3] = { obb.axis[0],obb.axis[1],obb.axis[2] };
//
//
//    // OBBの8つの頂点を計算
//    Vector3 vertices[8];
//    vertices[0] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], vExtents.x), DirectX::XMVectorScale(vAxis[1], vExtents.y)), DirectX::XMVectorScale(vAxis[2], vExtents.z)));
//    vertices[1] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], -vExtents.x), DirectX::XMVectorScale(vAxis[1], vExtents.y)), DirectX::XMVectorScale(vAxis[2], vExtents.z)));
//    vertices[2] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], vExtents.x), DirectX::XMVectorScale(vAxis[1], -vExtents.y)), DirectX::XMVectorScale(vAxis[2], vExtents.z)));
//    vertices[3] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], vExtents.x), DirectX::XMVectorScale(vAxis[1], vExtents.y)), DirectX::XMVectorScale(vAxis[2], -vExtents.z)));
//    vertices[4] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], -vExtents.x), DirectX::XMVectorScale(vAxis[1], -vExtents.y)), DirectX::XMVectorScale(vAxis[2], vExtents.z)));
//    vertices[5] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], -vExtents.x), DirectX::XMVectorScale(vAxis[1], vExtents.y)), DirectX::XMVectorScale(vAxis[2], -vExtents.z)));
//    vertices[6] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], vExtents.x), DirectX::XMVectorScale(vAxis[1], -vExtents.y)), DirectX::XMVectorScale(vAxis[2], -vExtents.z)));
//    vertices[7] = DirectX::XMVectorAdd(vCenter, DirectX::XMVectorAdd(DirectX::XMVectorAdd(DirectX::XMVectorScale(vAxis[0], -vExtents.x), DirectX::XMVectorScale(vAxis[1], -vExtents.y)), DirectX::XMVectorScale(vAxis[2], -vExtents.z)));
//
//    // 各平面に対して、頂点が全て平面の裏側にあるかをチェック
//    for (int i = 0; i < 6; i++) {
//        bool allOutside = true;
//        for (int j = 0; j < 8; j++) {
//            DirectX::XMVECTOR vPlane = DirectX::XMLoadFloat4(&planes[i]);
//            if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(vPlane, vertices[j])) >= 0.0f) {
//                allOutside = false;
//                break;
//            }
//        }
//        if (allOutside) {
//            return false; // OBBは視錐台外にある
//        }
//    }
//    return true; // OBBは視錐台内にある
//}
