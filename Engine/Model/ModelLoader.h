#pragma once
#include "GraphicsAPI.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "ModelStruct.h"
#include <string>
#include <vector>
#include <memory>
#include "EditorMode.h"

struct aiScene;
struct aiNode;
struct aiMesh;
namespace IFE
{
	class FBXModel;
	class ModelLoader
	{
	private:
		std::vector<std::unique_ptr<Node>> nodes_;
		std::vector<Bone> bones_;
		std::vector<Animation> animations_;
		std::string filename_;
		std::vector<std::string>m_BoneMapping_;
		bool objType_ = false;
	private:
		void ParseNodeRecursive(const aiScene* scene, aiNode* node, Node* targetParent = nullptr);
		Mesh* ProcessMesh(const aiScene* scene, aiMesh* mesh);
	public:
		FBXModel* FBXLoad(const std::string& fileName, const std::string& fileType = ".fbx", bool smooth = false);
	};
};
