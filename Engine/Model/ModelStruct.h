#pragma once
#include <vector>
#include <string>
#include "IFEMath.h"
#include "Quaternion.h"
#include "Material.h"

namespace IFE
{
	class Mesh;
	struct Node
	{
		std::string name;
		Matrix transform;
		Matrix globalTransform;
		std::vector<Mesh*> meshes;
		Node* parent = nullptr;
		Matrix animMat;
	};
	struct Bone
	{
		std::string name;
		Matrix invInitPose;
		Matrix finalMatrix;
		Bone* parent = nullptr;
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};
	struct NodeAnim
	{
		std::string name;
		std::vector<Vector3>position;
		std::vector<double>positionTime;
		std::vector<Vector3>scale;
		std::vector<double>scaleTime;
		std::vector<Quaternion>rotation;
		std::vector<double>rotationTime;
	};
	struct Animation
	{
		std::string name;
		double duration;
		double ticksPerSecond;
		std::vector<NodeAnim>channels;
		float endTime;
	};
}
