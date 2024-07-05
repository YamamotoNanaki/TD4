#pragma once
#include <d3d12.h>
#include <vector>
#include <memory>
#include <string>
#include "ConstStruct.h"
#include "IFEMath.h"
#include "IndexBuffer.h"
#include "Component.h"
#include "ModelStruct.h"
#include "AddModelSettings.h"

namespace IFE
{
	struct Triangle;
	class FBXModel :public Component
	{
		template<class T> using vector = std::vector<T>;
	private:
		AddModelSettings setting_;
		bool smooth_;
		Material* material_ = nullptr;

	public:
		std::vector<std::unique_ptr<Node>> nodes_{};
		std::vector<Bone> bones_{};
		std::vector<Animation> animations_{};
		std::vector<Matrix> transformMatrixs_{};
		std::string fileName_;

	public:
		void Initialize()override;
		void Update()override {};
		void Draw()override;
		void BoneTransform(float TimeInSeconds, uint8_t animNum);
		void SetSettings(const AddModelSettings& setting);
		void SetSmooth(bool smooth);
		void SetMaterial(Material* mat);
		Matrix* GetBoneTransform(std::string boneName);

		std::vector<Triangle> GetMeshColliderTriangle();
	private:
		void ReadNodeHeirarchy(float AnimationTime, Node* pNode, uint8_t animNum);

	public:
#ifdef InverseEditorMode
#else
		//void DebugGUI()override;
		bool ModelGUI(bool deleteFlag);
		void OutputComponent(nlohmann::json& json);
#endif
	};
};
