#include "Material.h"
#include "ImguiManager.h"
#include "TextureManager.h"
#include "JsonManager.h"
#include "Object3D.h"
#include "FBXModel.h"
#include "Mesh.h"
#include "GraphicsPipelineManager.h"

using namespace std;
using namespace IFE;

void IFE::Material::Initialize()
{
	materialBuffer_ = make_unique<ConstBuffer<ConstBufferMaterial>>();
	constMapMaterial_ = materialBuffer_->GetCBMapObject();

	MultipleMaterialCheck();
}

void IFE::Material::SetDefaultParameter()
{
	ambient_ = { 0.3f,0.3f,0.3f };
	diffuse_ = { 0.3f,0.3f,0.3f };
	specular_ = { 0.3f,0.3f,0.3f };
	alpha_ = 1.0f;
	name_ = "DefaultMaterial";
	constMapMaterial_->alpha = alpha_;
	constMapMaterial_->color = color_;
	constMapMaterial_->ambient = ambient_;
	constMapMaterial_->diffuse = diffuse_;
	constMapMaterial_->specular = specular_;
	tex_ = TextureManager::Instance()->GetTexture("white.png");
}

void IFE::Material::Update() {}

void IFE::Material::Draw()
{
	if (childMaterials_.size() == 0)
	{
		constMapMaterial_->alpha = alpha_;
		constMapMaterial_->color = color_;
		constMapMaterial_->ambient = ambient_;
		constMapMaterial_->diffuse = diffuse_;
		constMapMaterial_->specular = specular_;
		constMapMaterial_->bloom = bloom_;
		materialBuffer_->SetConstBuffView(2);
		if (tex_ == nullptr)
		{
			tex_ = TextureManager::Instance()->GetTexture("white.png");
		}
		tex_->SetTexture(5);
	}
	else
	{
		dynamic_cast<FBXModel*>(objectPtr_->GetModel())->SetMaterial(this);
	}
}

bool IFE::Material::ChildDraw(std::string name, uint32_t meshNum)
{
	if (!this)return false;
	if (childMaterials_.size() == 0)return false;
	for (auto& itr : childMaterials_)
	{
		if (itr.first.name == name && itr.first.meshNum == meshNum)
		{
			itr.first.constMapMaterial_->alpha = itr.first.alpha;
			itr.first.constMapMaterial_->color = itr.first.color;
			itr.first.constMapMaterial_->ambient = itr.first.ambient;
			itr.first.constMapMaterial_->diffuse = itr.first.diffuse;
			itr.first.constMapMaterial_->specular = itr.first.specular;
			itr.first.constMapMaterial_->bloom = itr.first.bloom;
			itr.second->SetConstBuffView(2);
			if (itr.first.tex == nullptr)
			{
				itr.first.tex = TextureManager::Instance()->GetTexture("white.png");
			}
			itr.first.tex->SetTexture(5);
			return true;
		}
	}
	return false;
}

void IFE::Material::SetTexture(Texture* texture)
{
	tex_ = texture;
}

Float4 IFE::Material::GetColor()
{
	return color_;
}

void IFE::Material::SetColor(const Float4& c)
{
	color_ = c;
}

Float4 IFE::Material::GetColor255()
{
	Float4 c = { color_.x * 255.f,color_.y * 255.f,color_.z * 255.f,color_.w * 255.f };
	return c;
}

void IFE::Material::SetColor255(const Float4& c)
{
	color_ = { c.x / 255.f,c.y / 255.f,c.z / 255.f,c.w / 255.f };
}

void IFE::Material::Copy(Component* component)
{
	Material* m = dynamic_cast<Material*>(component);
	if (m == nullptr)return;
	ambient_ = m->ambient_;
	diffuse_ = m->diffuse_;
	specular_ = m->specular_;
	color_ = m->color_;
	tex_ = m->tex_;
	bloom_ = m->bloom_;
}

void IFE::Material::SetMaterial(MaterialParams mat)
{
	ambient_ = mat.ambient;
	diffuse_ = mat.diffuse;
	specular_ = mat.specular;
	color_ = mat.color;
	alpha_ = mat.alpha;
	tex_ = mat.tex;
	bloom_ = mat.bloom;
}

MaterialParams IFE::Material::GetMaterial()
{
	MaterialParams m;
	m.ambient = ambient_;
	m.diffuse = diffuse_;
	m.specular = specular_;
	m.color = color_;
	m.alpha = alpha_;
	m.tex = tex_;
	m.bloom = bloom_;
	return m;
}

void IFE::Material::MultipleMaterialCheck()
{
	auto old = multipleMat_;
	multipleMat_ = false;
	if (!objectPtr_)return;
	auto fbx = dynamic_cast<FBXModel*>(objectPtr_->GetModel());

	if (!fbx)return;

	uint32_t meshNum = 0;
	for (auto& node : fbx->nodes_)
	{
		if (node->meshes.size() == 0)continue;
		for (size_t i = 0; i < node->meshes.size(); i++);
		{
			meshNum++;
			if (meshNum > 1)
			{
				break;
			}
		}
		if (meshNum > 1)
		{
			break;
		}
	}

	if (meshNum == 1)return;
	multipleMat_ = true;
	if (old)return;
	childMaterials_.clear();
	ChildMaterial m;
	for (auto& node : fbx->nodes_)
	{
		size_t i = 0;
		for (auto mesh : node->meshes)
		{
			childMaterials_.push_back({ m,std::move(std::make_unique<ConstBuffer<ConstBufferMaterial>>()) });
			auto& mat = childMaterials_.back().first;
			mat.name = node->name;
			mat.meshNum = uint32_t(i);
			auto prm = mesh->GetMaterial();
			mat.alpha = prm.alpha;
			mat.bloom = prm.bloom;
			mat.ambient = prm.ambient;
			mat.specular = prm.specular;
			mat.diffuse = prm.diffuse;
			mat.color = prm.color;
			mat.tex = prm.tex;
			mat.constMapMaterial_ = childMaterials_.back().second->GetCBMapObject();
			i++;
		}
	}
}

#ifdef InverseEditorMode
#else
void IFE::Material::ChildGUI(ChildMaterial& mat)
{
	ImguiManager* im = ImguiManager::Instance();
	string s = mat.name + to_string(mat.meshNum);
	if (im->NewTreeNode(s))
	{
		std::function<void(std::string)> guiFunc2 = [&](std::string textureName)
			{
				mat.tex = TextureManager::Instance()->GetTexture(textureName);
			};
		im->CheckBoxGUI(&mat.bloom, "bllom");
		im->ColorEdit4GUI(&mat.color, "color");
		im->DragFloat3GUI(&mat.ambient, "ambient");
		im->DragFloat3GUI(&mat.diffuse, "diffuse");
		im->DragFloat3GUI(&mat.specular, "specular");
		im->ChangeTextureGUI(guiFunc2);
		im->EndTreeNode();
	}
}
void IFE::Material::ComponentDebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(std::string)> guiFunc2 = [&](std::string textureName)
		{
			tex_ = TextureManager::Instance()->GetTexture(textureName);
		};

	if (im->ButtonGUI(U8("複数マテリアルかどうか確認する")))
	{
		MultipleMaterialCheck();
	}

	if (!multipleMat_)
	{
		im->CheckBoxGUI(&bloom_, "bllom");
		im->ColorEdit4GUI(&color_, "color");
		im->DragFloat3GUI(&ambient_, "ambient");
		im->DragFloat3GUI(&diffuse_, "diffuse");
		im->DragFloat3GUI(&specular_, "specular");
		im->ChangeTextureGUI(guiFunc2);
	}
	else
	{
		for (auto& itr : childMaterials_)
		{
			ChildGUI(itr.first);
		}
	}

	if (im->NewTreeNode(U8("シェーダー")))
	{
		string s = GraphicsPipelineManager::Instance()->GetGraphicsPipelineGUI();
		if (im->ButtonGUI(U8("変更")))
		{
			objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline(s);
		}
		im->EndTreeNode();
	}
}

void IFE::Material::OutputComponent(nlohmann::json& j)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat4(j["color"], color_);
	jm->OutputFloat3(j["ambient"], ambient_);
	jm->OutputFloat3(j["diffuse"], diffuse_);
	jm->OutputFloat3(j["specular"], specular_);
	j["terxtureName"] = tex_->texName_;
	j["bloom"] = bloom_;
	j["multipleMat"] = multipleMat_;
	j["pipeline"] = objectPtr_->gp_->name_;
	if (multipleMat_)
	{
		for (size_t i = 0; childMaterials_.size(); i++)
		{
			OutputChild(j["child"][i], childMaterials_[i].first);
			i++;
		}
	}
}
void IFE::Material::OutputChild(nlohmann::json& json, ChildMaterial& mat)
{
	JsonManager* jm = JsonManager::Instance();
	jm->OutputFloat4(json["color"], mat.color);
	jm->OutputFloat3(json["ambient"], mat.ambient);
	jm->OutputFloat3(json["diffuse"], mat.diffuse);
	jm->OutputFloat3(json["specular"], mat.specular);
	json["terxtureName"] = mat.tex->texName_;
	json["bloom"] = mat.bloom;
	json["name"] = mat.name;
	json["meshNum"] = mat.meshNum;
}
#endif

void IFE::Material::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	color_ = j->InputFloat4(json["color"]);
	ambient_ = j->InputFloat3(json["ambient"]);
	diffuse_ = j->InputFloat3(json["diffuse"]);
	specular_ = j->InputFloat3(json["specular"]);
	SetTexture(TextureManager::Instance()->GetTexture(json["terxtureName"]));
	materialBuffer_ = make_unique<ConstBuffer<ConstBufferMaterial>>();
	constMapMaterial_ = materialBuffer_->GetCBMapObject();
	bloom_ = json["bloom"];
	multipleMat_ = json["multipleMat"];
	string s;
	if (j->GetData(json, "pipeline", s))
	{
		objectPtr_->gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline(s);
	}

	if (multipleMat_)
	{
		for (auto& itr : json["child"])
		{
			LoadingChild(itr);
		}
	}
}

void IFE::Material::LoadingChild(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	ChildMaterial m;
	childMaterials_.push_back({ m,std::move(std::make_unique<ConstBuffer<ConstBufferMaterial>>()) });
	auto& mat = childMaterials_.back().first;

	mat.color = j->InputFloat4(json["color"]);
	mat.ambient = j->InputFloat3(json["ambient"]);
	mat.diffuse = j->InputFloat3(json["diffuse"]);
	mat.specular = j->InputFloat3(json["specular"]);
	mat.tex = TextureManager::Instance()->GetTexture(json["terxtureName"]);
	mat.bloom = json["bloom"];
	mat.name = json["name"];
	mat.meshNum = json["meshNum"];
	mat.constMapMaterial_ = childMaterials_.back().second->GetCBMapObject();
}
