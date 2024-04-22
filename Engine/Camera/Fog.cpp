#include "Fog.h"
#include "JsonManager.h"

void IFE::Fog::Initialize()
{
	fogBuffer_ = std::make_unique<ConstBuffer<ConstBufferFog>>();
	constMapFog_ = fogBuffer_->GetCBMapObject();
	componentName_ = "Fog";
}

void IFE::Fog::Update()
{
	constMapFog_->color = color_;
	constMapFog_->fogFar = fogFar_;
	constMapFog_->fogNear = fogNear_;
	constMapFog_->fogFlag = (uint32_t)fogFlag_;
}

void IFE::Fog::Draw()
{
	fogBuffer_->SetConstBuffView(4);
}

void IFE::Fog::Copy(Component* component)
{
	Fog* t = dynamic_cast<Fog*>(component);
	if (t == nullptr)return;
	color_ = t->color_;
	fogFar_ = t->fogFar_;
	fogNear_ = t->fogNear_;
	fogFlag_ = t->fogFlag_;
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
void IFE::Fog::ComponentDebugGUI()
{
	auto im = ImguiManager::Instance();

	im->CheckBoxGUI(&fogFlag_, "Use Fog");
	if (fogFlag_)
	{
		im->ColorEdit4GUI(&color_, "Fog Color");
		im->DragFloatGUI(&fogFar_, "Fog Far");
		im->DragFloatGUI(&fogNear_, "Fog Near");
	}
}

void IFE::Fog::OutputComponent(nlohmann::json& json)
{
	auto jm = JsonManager::Instance();
	jm->OutputFloat4(json["color"], color_);
	json["fogFar"] = fogFar_;
	json["fogNear"] = fogNear_;
	json["fogFlag"] = fogFlag_;
}
#endif

void IFE::Fog::LoadingComponent(nlohmann::json& json)
{
	auto jm = JsonManager::Instance();
	color_ = jm->InputFloat4(json["color"]);
	fogFar_ = json["fogFar"];
	fogNear_ = json["fogNear"];
	fogFlag_ = json["fogFlag"];
}
