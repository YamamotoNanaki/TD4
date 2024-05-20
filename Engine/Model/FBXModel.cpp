#include "FBXModel.h"
#include "GraphicsAPI.h"
#include "Util.h"
#include "Mesh.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "CollisionPrimitive.h"
#include <cassert>

using namespace IFE;
using namespace std;

void IFE::FBXModel::Initialize()
{
	for (unique_ptr<Node>& node : nodes_)
	{
		for (auto mesh : node->meshes)
		{
			mesh->Initialize();
		}
	}
}

void IFE::FBXModel::Draw()
{
	for (unique_ptr<Node>& node : nodes_)
	{
		for (auto mesh : node->meshes)
		{
			mesh->Draw();
		}
	}
}

//アニメーション
static const NodeAnim* FindNodeAnim(const Animation* pAnimation, const string& NodeName)
{
	for (uint32_t i = 0; i < pAnimation->channels.size(); i++)
	{
		const NodeAnim* pNodeAnim = &pAnimation->channels[i];

		if (pNodeAnim->name == NodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}

static uint32_t findScaling(float AnimationTime, const NodeAnim* pNodeAnim)
{
	assert(pNodeAnim->scale.size() > 0);

	for (uint32_t i = 0; i < pNodeAnim->scale.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->scaleTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}

static void CalcInterpolatedScaling(Vector3& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	if (pNodeAnim->scale.size() == 1) {
		Out = pNodeAnim->scale[0];
		return;
	}

	uint32_t ScalingIndex = findScaling(AnimationTime, pNodeAnim);
	uint32_t NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->scale.size());
	float DeltaTime = (float)(pNodeAnim->scaleTime[NextScalingIndex] - pNodeAnim->scaleTime[ScalingIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->scaleTime[ScalingIndex]) / DeltaTime);
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const Vector3& Start = pNodeAnim->scale[ScalingIndex];
	const Vector3& End = pNodeAnim->scale[NextScalingIndex];
	Vector3 Delta = End - Start;
	Out = Start + Factor * Delta;
}

static uint32_t findRotation(float AnimationTime, const NodeAnim* pNodeAnim)
{
	assert(pNodeAnim->rotation.size() > 0);

	for (uint32_t i = 0; i < pNodeAnim->rotation.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->rotationTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}

static void CalcInterpolatedRotation(Quaternion& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->rotation.size() == 1) {
		Out = pNodeAnim->rotation[0];
		return;
	}

	uint32_t RotationIndex = findRotation(AnimationTime, pNodeAnim);
	uint32_t NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->rotation.size());
	float DeltaTime = (float)(pNodeAnim->rotationTime[NextRotationIndex] - pNodeAnim->rotationTime[RotationIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->rotationTime[RotationIndex]) / DeltaTime);

	const Quaternion& StartRotationQ = pNodeAnim->rotation[RotationIndex];
	const Quaternion& EndRotationQ = pNodeAnim->rotation[NextRotationIndex];
	Out = SLerp(StartRotationQ, EndRotationQ, Factor);
	Out = normalize(Out);
}

static uint32_t findPosition(float AnimationTime, const NodeAnim* pNodeAnim)
{
	for (uint32_t i = 0; i < pNodeAnim->position.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->positionTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}

static void CalcInterpolatedPosition(Vector3& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	if (pNodeAnim->position.size() == 1)
	{
		Out = pNodeAnim->position[0];
		return;
	}

	uint32_t PositionIndex = findPosition(AnimationTime, pNodeAnim);
	uint32_t NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->position.size());
	float DeltaTime = (float)(pNodeAnim->positionTime[NextPositionIndex] - pNodeAnim->positionTime[PositionIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->positionTime[PositionIndex]) / DeltaTime);
	const Vector3& Start = pNodeAnim->position[PositionIndex];
	const Vector3& End = pNodeAnim->position[NextPositionIndex];
	Vector3 Delta = End - Start;
	Out = Start + Factor * Delta;
}

void FBXModel::ReadNodeHeirarchy(float AnimationTime, Node* pNode, uint8_t animNum)
{
	string NodeName = pNode->name;

	const Animation* pAnimation = &animations_[animNum];

	Matrix NodeTransformation = pNode->transform;

	const NodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// スケーリングを補間し、スケーリング変換行列を生成する
		Vector3 Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		Matrix ScalingM;
		ScalingM = MatrixScaling(Scaling.x, Scaling.y, Scaling.z);

		// 回転を補間し、回転変換行列を生成する
		Quaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Matrix RotationM = RotateMatrix(RotationQ);

		// 移動を補間し、移動変換行列を生成する
		Vector3 Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		Matrix TranslationM;
		TranslationM = MatrixTranslation(Translation.x, Translation.y, Translation.z);

		// これら上記の変換を合成する
		NodeTransformation = ScalingM * RotationM * TranslationM;
	}

	if (pNode->parent)
	{
		pNode->animMat = NodeTransformation * pNode->parent->animMat;
	}
	else
	{
		pNode->animMat = NodeTransformation;
	}

	Matrix mat = pNode->animMat;

	Bone* bone = nullptr;
	for (auto& itr : bones_)
	{
		if (itr.name == NodeName)
		{
			bone = &itr;
			break;
		}
	}
	if (bone)
	{
		Matrix* boneoffset = &bone->invInitPose;
		Matrix tmpfinal = *boneoffset * mat;
		bone->finalMatrix = tmpfinal;
	}
}

void FBXModel::BoneTransform(float TimeInSeconds, uint8_t animNum)
{
	Matrix Identity;

	float TicksPerSecond = (float)animations_[animNum].ticksPerSecond != 0 ?
		(float)animations_[animNum].ticksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = (float)fmod(TimeInTicks, animations_[animNum].duration);

	for (uint32_t i = 0; i < nodes_.size(); i++) {
		ReadNodeHeirarchy(AnimationTime, nodes_[i].get(), animNum);
	}
}

void IFE::FBXModel::SetSettings(const AddModelSettings& s)
{
	setting_ = s;
}

void IFE::FBXModel::SetSmooth(bool smooth)
{
	smooth_ = smooth;
}

std::vector<Triangle> IFE::FBXModel::GetMeshColliderTriangle()
{
	std::vector<Triangle>triangles;
	size_t start = 0;
	for (auto& node : nodes_)
	{
		for (auto itr : node->meshes)
		{
			vector<uint32_t> indices = itr->GetIndexArray();
			vector<Vertex> vertices = itr->GetVertexArray();
			size_t triangleNum = indices.size() / 3;
			triangles.resize(triangles.size() + triangleNum);
			for (uint32_t i = 0; i < triangleNum; i++)
			{
				Triangle& tri = triangles[start + i];
				uint32_t idx0 = indices[i * 3 + 0];
				uint32_t idx1 = indices[i * 3 + 1];
				uint32_t idx2 = indices[i * 3 + 2];

				tri.p0 = { vertices[idx0].pos.x,vertices[idx0].pos.y,vertices[idx0].pos.z };
				tri.p1 = { vertices[idx1].pos.x,vertices[idx1].pos.y,vertices[idx1].pos.z };
				tri.p2 = { vertices[idx2].pos.x,vertices[idx2].pos.y,vertices[idx2].pos.z };

				tri.ComputeNormal();
			}
			start += triangleNum;
		}
	}

	return triangles;
}

#ifdef InverseEditorMode
#else
//void IFE::FBXModel::DebugGUI()
//{
//	ImguiManager* imgui = ImguiManager::Instance();
//	if (imgui->NewTreeNode(fileName_))
//	{
//		for (unique_ptr<Node>& node : nodes_)
//		{
//			for (auto mesh : node->meshes)
//			{
//				mesh->DebugGUI();
//			}
//		}
//		imgui->EndTreeNode();
//	}
//}

bool IFE::FBXModel::ModelGUI(bool deleteFlag)
{
	ImguiManager* imgui = ImguiManager::Instance();
	if (imgui->NewTreeNode(fileName_))
	{
		for (unique_ptr<Node>& node : nodes_)
		{
			uint32_t num = 0;
			for (auto mesh : node->meshes)
			{
				mesh->DebugGUI(num++);
			}
		}
		if (deleteFlag)
		{
			if (imgui->ButtonGUI(U8("削除")))
			{
				imgui->EndTreeNode();
				return true;
			}
		}
		imgui->EndTreeNode();
	}
	return false;
}

void IFE::FBXModel::OutputComponent(nlohmann::json& j)
{
	j["fileName"] = fileName_;
	j["settings"] = (uint32_t)setting_;
	j["smooth"] = smooth_;
}
#endif