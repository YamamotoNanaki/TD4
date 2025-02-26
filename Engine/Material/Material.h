#pragma once
#include "Texture.h"
#include "IFEMath.h"
#include "ConstBuffer.h"
#include "ConstStruct.h"
#include "Component.h"
#include <string>
#include <memory>
#include <utility>
#include "GraphicsPipelineManager.h"

namespace IFE
{
	struct MaterialParams
	{
		Float3 ambient = { 0.3f,0.3f,0.3f };
		Float3 diffuse = { 0.3f,0.3f,0.3f };
		Float3 specular = { 0.3f,0.3f,0.3f };
		Float4 color = { 1,1,1,1 };
		float alpha = 1.0f;
		Texture* tex = nullptr;
		bool bloom = false;
	};
	class Material : public Component
	{
		using Component::Component;
	private:
		std::string name_ = "";
	public:
		Float3 ambient_ = { 0.3f,0.3f,0.3f };
		Float3 diffuse_ = { 0.3f,0.3f,0.3f };
		Float3 specular_ = { 0.3f,0.3f,0.3f };
	private:
		bool multipleMat_;
		struct ChildMaterial
		{
			std::string name = "";
			uint32_t meshNum = uint32_t(-1);
			Float3 ambient = { 0.3f,0.3f,0.3f };
			Float3 diffuse = { 0.3f,0.3f,0.3f };
			Float3 specular = { 0.3f,0.3f,0.3f };
			Texture* tex = nullptr;
			bool bloom = false;
			Float4 color = { 1,1,1,1 };
			float alpha = 1.0f;
			ConstBufferMaterial* constMapMaterial_ = nullptr;
		};

		GraphicsPipeline* gp_ = nullptr;

	public:
		std::vector<std::pair<ChildMaterial, std::unique_ptr<ConstBuffer<ConstBufferMaterial>>>> childMaterials_;
		Float4 color_ = { 1,1,1,1 };
		float alpha_ = 1.0f;
		bool bloom_ = false;
		Texture* tex_ = nullptr;

		std::unique_ptr<ConstBuffer<ConstBufferMaterial>> materialBuffer_;
		ConstBufferMaterial* constMapMaterial_ = nullptr;

	public:
		void Initialize();
		void SetDefaultParameter();
		void Update();
		void Draw();
		bool ChildDraw(std::string name, uint32_t meshNum);
		void SetTexture(Texture* texture);
		Float4 GetColor();
		void SetColor(const Float4& color);
		Float4 GetColor255();
		void SetColor255(const Float4& color);
		void Copy(Component* component);
		void SetMaterial(MaterialParams mat);
		MaterialParams GetMaterial();
		void MultipleMaterialCheck();
#ifdef InverseEditorMode
#else
		void DebugInitialize()override;
		void ChildGUI(ChildMaterial& mat);
		void ComponentDebugGUI();
		void OutputComponent(nlohmann::json& json)override;
		void OutputChild(nlohmann::json& json, ChildMaterial& mat);
#endif
		void LoadingComponent(nlohmann::json& json)override;
		void LoadingChild(nlohmann::json& json);
	};
}
