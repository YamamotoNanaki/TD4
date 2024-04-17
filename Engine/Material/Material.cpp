#include "Material.h"
#include "ImguiManager.h"
#include "TextureManager.h"
#include "JsonManager.h"

using namespace std;
using namespace IFE;

void IFE::Material::Initialize()
{
	materialBuffer_ = make_unique<ConstBuffer<ConstBufferMaterial>>();
	constMapMaterial_ = materialBuffer_->GetCBMapObject();
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

#ifdef NDEBUG
#else
void IFE::Material::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(std::string)> guiFunc2 = [&](std::string textureName)
		{
			tex_ = TextureManager::Instance()->GetTexture(textureName);
		};
	std::function<void(void)> guiFunc = [&]()
		{
			im->CheckBoxGUI(&bloom_, "bllom");
			im->ColorEdit4GUI(&color_, "color");
			im->DragFloat3GUI(&ambient_, "ambient");
			im->DragFloat3GUI(&diffuse_, "diffuse");
			im->DragFloat3GUI(&specular_, "specular");
			im->ChangeTextureGUI(guiFunc2);
		};
	std::function<void(void)> deleteFunc = [&]()
		{
			componentDeleteFlag_ = true;
		};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
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
}