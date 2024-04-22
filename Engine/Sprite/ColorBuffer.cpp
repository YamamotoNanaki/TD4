#include "ColorBuffer.h"
#include "ImguiManager.h"
#include "JsonManager.h"

using namespace IFE;
using namespace std;

void IFE::ColorBuffer::Initialize()
{
	if (particlePtr_)return;
	colorBuffer_ = make_unique<ConstBuffer<ConstBufferColor>>();
	constMapColor_ = colorBuffer_->GetCBMapObject();
}

void IFE::ColorBuffer::Draw()
{
	if (particlePtr_)return;
	constMapColor_->color = color_;
	colorBuffer_->SetConstBuffView(0);
}

void IFE::ColorBuffer::SetColor(const Float4& color)
{
	color_ = color;
}

void IFE::ColorBuffer::SetAlpha(float a)
{
	color_.w = a;
}

void IFE::ColorBuffer::SetColor(float r, float g, float b, float a)
{
	color_ = Float4(r,g,b,a);
}

Float4 IFE::ColorBuffer::GetColor()
{
	return color_;
}

#ifdef InverseEditorMode
#else
void IFE::ColorBuffer::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->ColorEdit4GUI(&color_, "color");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc, componentName_);
}

void IFE::ColorBuffer::OutputComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat4(json["color"], color_);
}
#endif

void IFE::ColorBuffer::LoadingComponent(nlohmann::json& json)
{
	JsonManager* j = JsonManager::Instance();
	color_ = j->InputFloat4(json["color"]);
	colorBuffer_ = make_unique<ConstBuffer<ConstBufferColor>>();
	constMapColor_ = colorBuffer_->GetCBMapObject();
}
