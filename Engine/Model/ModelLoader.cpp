#include "ModelLoader.h"
#include "FBXModel.h"
#include "TextureManager.h"
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace IFE;
using namespace std;

void IFE::ModelLoader::ParseNodeRecursive(const aiScene* scene, aiNode* node, Node* targetParent)
{
	unique_ptr<Node> node_ = make_unique<Node>();
	string name = node->mName.C_Str();
	node_->name = name;
	node_->transform.SetX(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4);
	node_->transform.SetY(node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4);
	node_->transform.SetZ(node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4);
	node_->transform.SetW(node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4);
	node_->globalTransform = node_->transform = MatrixTranspose(node_->transform);
	for (uint32_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		node_->meshes.push_back(this->ProcessMesh(scene, mesh));
	}
	nodes_.push_back(std::move(node_));
	Node* parent;
	parent = nodes_.back().get();

	if (targetParent != nullptr)
	{
		parent->parent = targetParent;
		targetParent->globalTransform *= parent->globalTransform;
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		this->ParseNodeRecursive(scene, node->mChildren[i], parent);
	}
}

Mesh* IFE::ModelLoader::ProcessMesh(const aiScene* scene, aiMesh* mesh)
{
	MaterialParams mat;
	if (objType_)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0]) {
				vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
				vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
			}

			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		Mesh* mesh_ = new Mesh;
		aiTextureType type = aiTextureType_DIFFUSE;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);
			string f = /*filename + "/" +*/ str.C_Str();
			while (f.find("\\") != std::string::npos)
			{
				f = f.substr(f.find("\\") + 1);
			}
			/*f = filename + "/" + f;*/
			TextureManager::Instance()->LoadTexture(f);
		}

		aiColor3D col;
		material->Get(AI_MATKEY_COLOR_AMBIENT, col);
		mat.ambient.x = col.r;
		mat.ambient.y = col.g;
		mat.ambient.z = col.b;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, col);
		mat.diffuse.x = col.r;
		mat.diffuse.y = col.g;
		mat.diffuse.z = col.b;
		material->Get(AI_MATKEY_COLOR_SPECULAR, col);
		mat.specular.x = col.r;
		mat.specular.y = col.g;
		mat.specular.z = col.b;
		material->Get(AI_MATKEY_OPACITY, mat.alpha);

		mesh_->GetVertex()->SetVerticle(vertices);
		mesh_->GetIB()->SetIndex(indices);
		mesh_->Initialize();
		return mesh_;
	}
	else
	{
		std::vector<VertexBone> vertices;
		std::vector<uint32_t> indices;

		struct Weight
		{
			uint32_t vertexID;
			float weight;
		};

		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
			VertexBone vertex;

			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0]) {
				vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
				vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
			}

			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		Mesh* mesh_ = new Mesh;
		aiTextureType type = aiTextureType_DIFFUSE;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);
			string f = /*filename + "/" +*/ str.C_Str();
			while (f.find("\\") != std::string::npos)
			{
				f = f.substr(f.find("\\") + 1);
			}
			/*f = filename + "/" + f;*/
			mat.tex = TextureManager::Instance()->LoadTexture(f);
		}

		//É{Å[Éì
		std::vector<std::list<Weight>>lists(vertices.size());
		if (bones_.size() == 0)
		{
			for (uint32_t i = 0; i < mesh->mNumBones; i++)
			{
				Bone b(mesh->mBones[i]->mName.C_Str());
				b.invInitPose.SetX(mesh->mBones[i]->mOffsetMatrix[0][0], mesh->mBones[i]->mOffsetMatrix[0][1], mesh->mBones[i]->mOffsetMatrix[0][2], mesh->mBones[i]->mOffsetMatrix[0][3]);
				b.invInitPose.SetY(mesh->mBones[i]->mOffsetMatrix[1][0], mesh->mBones[i]->mOffsetMatrix[1][1], mesh->mBones[i]->mOffsetMatrix[1][2], mesh->mBones[i]->mOffsetMatrix[1][3]);
				b.invInitPose.SetZ(mesh->mBones[i]->mOffsetMatrix[2][0], mesh->mBones[i]->mOffsetMatrix[2][1], mesh->mBones[i]->mOffsetMatrix[2][2], mesh->mBones[i]->mOffsetMatrix[2][3]);
				b.invInitPose.SetW(mesh->mBones[i]->mOffsetMatrix[3][0], mesh->mBones[i]->mOffsetMatrix[3][1], mesh->mBones[i]->mOffsetMatrix[3][2], mesh->mBones[i]->mOffsetMatrix[3][3]);
				b.invInitPose = b.finalMatrix = MatrixTranspose(b.invInitPose);
				bones_.push_back(b);
			}
		}
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			for (uint32_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				Weight w;
				w.vertexID = i;
				w.weight = mesh->mBones[i]->mWeights[j].mWeight;
				lists[mesh->mBones[i]->mWeights[j].mVertexId].push_back(w);
			}
		}

		for (int32_t i = 0; i < vertices.size(); i++)
		{
			auto& l = lists[i];

			l.sort([](auto const& lhs, auto const& rhs) {
				return lhs.weight > rhs.weight;
				});

			int32_t w = 0;
			for (auto& s : l)
			{
				vertices[i].boneIndex[w] = s.vertexID;
				vertices[i].boneWeight[w] = s.weight;
				if (++w >= MAX_BONE_INDICES)
				{
					float we = 0.0f;
					for (int32_t j = 1; j < MAX_BONE_INDICES; j++)
					{
						we += vertices[i].boneWeight[j];
					}
					vertices[i].boneWeight[0] = 1.0f - we;
					break;
				}
			}
		}

		aiColor3D col;
		material->Get(AI_MATKEY_COLOR_AMBIENT, col);
		mat.ambient.x = col.r;
		mat.ambient.y = col.g;
		mat.ambient.z = col.b;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, col);
		mat.diffuse.x = col.r;
		mat.diffuse.y = col.g;
		mat.diffuse.z = col.b;
		material->Get(AI_MATKEY_COLOR_SPECULAR, col);
		mat.specular.x = col.r;
		mat.specular.y = col.g;
		mat.specular.z = col.b;
		material->Get(AI_MATKEY_OPACITY, mat.alpha);

		mesh_->GetVertexBone()->SetVerticle(vertices);
		mesh_->GetIB()->SetIndex(indices);
		mesh_->Initialize();
		mesh_->componentName_ = mesh->mName.C_Str();
		mesh_->SetMaterial(mat);
		return mesh_;
	}
}

FBXModel* IFE::ModelLoader::FBXLoad(const std::string& fileName, const std::string& fileType, bool smooth)
{
	Assimp::Importer importer;
	string f = "Data/Resources/Model/";
	if (fileType == ".obj")objType_ = true;
	else objType_ = false;
	f += fileName + "/" + fileName + fileType;
	filename_ = fileName;
	uint32_t sf = 0;
	if (smooth == true)
	{
		sf = aiProcess_GenSmoothNormals;
	}
	const aiScene* scene = importer.ReadFile(f,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | aiProcess_ConvertToLeftHanded |
		sf
	);

	if (scene == nullptr)
	{
		OutputDebugStringA(importer.GetErrorString());
		return nullptr;
	}

	ParseNodeRecursive(scene, scene->mRootNode);

	for (uint32_t i = 0; i < scene->mNumAnimations; i++)
	{
		Animation a;
		a.name = scene->mAnimations[i]->mName.C_Str();
		a.duration = scene->mAnimations[i]->mDuration;
		a.ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
		a.endTime = float(a.duration / a.ticksPerSecond);
		for (uint32_t j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			NodeAnim n;
			n.name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
			for (uint32_t k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
			{
				Vector3 p;
				double t;
				p.x = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x;
				p.y = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y;
				p.z = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z;
				t = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
				n.position.push_back(p);
				n.positionTime.push_back(t);
			}
			for (uint32_t k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++)
			{
				Vector3 s;
				double t;
				s.x = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x;
				s.y = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y;
				s.z = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z;
				t = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
				n.scale.push_back(s);
				n.scaleTime.push_back(t);
			}
			for (uint32_t k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)
			{
				Quaternion q;
				double t;
				q.x = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x;
				q.y = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y;
				q.z = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z;
				q.w = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w;
				t = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
				n.rotation.push_back(q);
				n.rotationTime.push_back(t);
			}
			a.channels.push_back(n);
		}
		animations_.push_back(a);
	}

	FBXModel* fbx = new FBXModel;
	for (std::unique_ptr<Node>& node : nodes_)
		fbx->nodes_.push_back(std::move(node));
	for (auto& b : bones_)
		fbx->bones_.push_back(b);
	for (auto& a : animations_)
		fbx->animations_.push_back(a);
	fbx->SetSmooth(smooth);

	nodes_.clear();

	fbx->fileName_ = filename_;
	return fbx;
}
